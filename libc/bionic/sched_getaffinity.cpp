/*
 * Copyright (C) 2010 The Android Open Source Project
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

#define _GNU_SOURCE 1
#include <sched.h>
#include <string.h>

#if defined(__FreeBSD__)

#include <errno.h>
#include <stdint.h>

// bionic-freebsd: FreeBSD has cpuset_getaffinity(2) and cpuset_setaffinity(2) in place of the
// Linux system calls, so this file has sched_setaffinity too. The values are from FreeBSD
// sys/sys/cpuset.h: CPU_LEVEL_WHICH is the mask of the thread itself, and CPU_WHICH_TIDPID takes
// a thread id or a process id (id -1 is the calling thread). FreeBSD id_t is 64 bits wide.
static constexpr int kCpuLevelWhich = 3;
static constexpr int kCpuWhichTidPid = 9;

extern "C" int __cpuset_getaffinity(int, int, int64_t, size_t, void*);
extern "C" int __cpuset_setaffinity(int, int, int64_t, size_t, const void*);

// Linux reports a set that is too small, and an empty set, as EINVAL. FreeBSD uses ERANGE and
// EDEADLK.
static int fix_errno(int rc) {
  if (rc == -1 && (errno == ERANGE || errno == EDEADLK)) errno = EINVAL;
  return rc;
}

// The kernel clears the bytes of the set that it does not write.
int sched_getaffinity(pid_t pid, size_t set_size, cpu_set_t* set) {
  return fix_errno(__cpuset_getaffinity(kCpuLevelWhich, kCpuWhichTidPid, pid == 0 ? -1 : pid,
                                        set_size, set));
}

int sched_setaffinity(pid_t pid, size_t set_size, const cpu_set_t* set) {
  return fix_errno(__cpuset_setaffinity(kCpuLevelWhich, kCpuWhichTidPid, pid == 0 ? -1 : pid,
                                        set_size, set));
}

#else

extern "C" int __sched_getaffinity(pid_t, size_t, cpu_set_t*);

int sched_getaffinity(pid_t pid, size_t set_size, cpu_set_t* set) {
  int rc = __sched_getaffinity(pid, set_size, set);
  if (rc == -1) {
    return -1;
  }

  // Clear any bytes the kernel didn't touch.
  // (The kernel returns the number of bytes written on success.)
  memset(reinterpret_cast<char*>(set) + rc, 0, set_size - rc);
  return 0;
}

#endif
