/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/ioctl.h>
#include <stdarg.h>

#if defined(__FreeBSD__)
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/ashmem.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sysctl.h>
#endif

extern "C" int __ioctl(int, int, void *);

#if defined(__FreeBSD__)
// FreeBSD has no ashmem device.  A memfd is a FreeBSD shm object, and each
// ashmem command on it is one fcntl(2), with the answer that an Android
// kernel gives for a memfd.  Any other file gives ENOTTY.

static bool ashmem_kinfo(int fd, kinfo_file* kif) {
  memset(kif, 0, sizeof(*kif));
  kif->kf_structsize = sizeof(*kif);
  if (fcntl(fd, F_KINFO, kif) == -1) return false;
  if (kif->kf_type != KF_TYPE_SHM) {
    errno = ENOTTY;
    return false;
  }
  return true;
}

// The seal commands give EINVAL for a file that is not a shm object.
static int ashmem_seal_result(int rc) {
  if (rc == -1 && errno == EINVAL) errno = ENOTTY;
  return rc;
}

static int ashmem_ioctl(int fd, unsigned request, void* arg) {
  kinfo_file kif;
  const char* name;
  uintptr_t prot;
  int seals;

  switch (request) {
    case ASHMEM_GET_NAME:
      if (!ashmem_kinfo(fd, &kif)) return -1;
      name = kif.kf_path;
      if (strncmp(name, "memfd:", 6) == 0) name += 6;
      strlcpy(static_cast<char*>(arg), name, ASHMEM_NAME_LEN);
      return 0;
    case ASHMEM_GET_SIZE:
      if (!ashmem_kinfo(fd, &kif)) return -1;
      if (kif.kf_un.kf_file.kf_file_size > INT_MAX) {
        errno = EOVERFLOW;
        return -1;
      }
      return static_cast<int>(kif.kf_un.kf_file.kf_file_size);
    case ASHMEM_GET_FILE_ID:
      if (!ashmem_kinfo(fd, &kif)) return -1;
      *static_cast<unsigned long*>(arg) = kif.kf_un.kf_file.kf_file_fileid;
      return 0;
    case ASHMEM_GET_PROT_MASK:
      seals = ashmem_seal_result(fcntl(fd, F_GET_SEALS));
      if (seals == -1) return -1;
      if ((seals & (F_SEAL_WRITE | F_SEAL_FUTURE_WRITE)) != 0) return PROT_READ | PROT_EXEC;
      return PROT_READ | PROT_WRITE | PROT_EXEC;
    case ASHMEM_SET_PROT_MASK:
      prot = reinterpret_cast<uintptr_t>(arg);
      if ((prot & ~static_cast<uintptr_t>(PROT_READ | PROT_WRITE | PROT_EXEC)) != 0) {
        errno = EINVAL;
        return -1;
      }
      // Only the write permission can change, and only once: its removal
      // is the F_SEAL_FUTURE_WRITE seal.
      if ((prot & PROT_WRITE) == 0) {
        return ashmem_seal_result(fcntl(fd, F_ADD_SEALS, F_SEAL_FUTURE_WRITE));
      }
      seals = ashmem_seal_result(fcntl(fd, F_GET_SEALS));
      if (seals == -1) return -1;
      if ((seals & (F_SEAL_WRITE | F_SEAL_FUTURE_WRITE)) != 0) {
        errno = EINVAL;
        return -1;
      }
      return 0;
    case ASHMEM_SET_NAME:
    case ASHMEM_SET_SIZE:
      // A memfd gets its name and its size when it is created.
      if (ashmem_seal_result(fcntl(fd, F_GET_SEALS)) == -1) return -1;
      errno = EINVAL;
      return -1;
    case ASHMEM_PIN:
      // The kernel never purges a memfd.
      if (ashmem_seal_result(fcntl(fd, F_GET_SEALS)) == -1) return -1;
      return ASHMEM_NOT_PURGED;
    case ASHMEM_UNPIN:
    case ASHMEM_PURGE_ALL_CACHES:
      if (ashmem_seal_result(fcntl(fd, F_GET_SEALS)) == -1) return -1;
      return 0;
    case ASHMEM_GET_PIN_STATUS:
      if (ashmem_seal_result(fcntl(fd, F_GET_SEALS)) == -1) return -1;
      return ASHMEM_IS_PINNED;
    default:
      return __ioctl(fd, request, arg);
  }
}
#endif

int ioctl(int fd, int request, ...) {
  va_list ap;
  va_start(ap, request);
  void* arg = va_arg(ap, void*);
  va_end(ap);
#if defined(__FreeBSD__)
  if (((static_cast<unsigned>(request) >> 8) & 0xff) == __ASHMEMIOC) {
    return ashmem_ioctl(fd, request, arg);
  }
#endif
  return __ioctl(fd, request, arg);
}
