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
#if defined(__aarch64__)
#include <arm_sme.h>
#endif
#include <sched.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/syscall.h>

#include "pthread_internal.h"

#include "platform/bionic/macros.h"
#include "private/bionic_defs.h"

#if defined(__FreeBSD__)
#include "private/freebsd_fork.h"

// FreeBSD makes a new process with rfork_ids(2), rfork(2), or vfork(2),
// and a new thread only with thr_new(2) (pthread_create()). Each process
// form of clone() uses one native call.

// FreeBSD <sys/unistd.h> rfork(2) flags.
#define FREEBSD_RFMEM (1 << 5)
#define FREEBSD_RFSIGSHARE (1 << 14)
#define FREEBSD_RFTSIGZMB (1 << 19)
#define FREEBSD_RFTSIGSHIFT 20

extern "C" pid_t __freebsd_clone(int flags, void* child_stack, int (*fn)(void*), void* arg,
                                 int use_vfork, rfork_ids* ids, int* child_tid);

// The rfork(2) flags for the clone() flags, or -1 for a form that FreeBSD
// cannot make with one call: threads (CLONE_THREAD, CLONE_SETTLS), the stores
// of the id in the parent (CLONE_PARENT_SETTID), name spaces,
// and the clear of a thread id at the exit of a child that shares memory.
static int clone_rfork_flags(int flags, bool has_fn, bool* use_vfork) {
  static constexpr int kAllowed = CSIGNAL | CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
                                  CLONE_VFORK | CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID |
                                  CLONE_DETACHED;
  if ((flags & ~kAllowed) != 0) return -1;

  *use_vfork = false;
  if ((flags & CLONE_VFORK) != 0) {
    // vfork(2): the child shares the memory and runs on its own stack, and
    // the parent waits for its exec or exit.
    if (!has_fn || (flags & ~CLONE_DETACHED) != (CLONE_VM | CLONE_VFORK | SIGCHLD)) return -1;
    *use_vfork = true;
    return 0;
  }

  int rfflags = FREEBSD_RFPROC;
  if ((flags & CLONE_VM) != 0) {
    if (!has_fn || (flags & (CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID)) != 0) return -1;
    rfflags |= FREEBSD_RFMEM;
  }
  if ((flags & CLONE_SIGHAND) != 0) {
    if ((flags & CLONE_VM) == 0) return -1;
    rfflags |= FREEBSD_RFSIGSHARE;
  }
  // FreeBSD shares or copies the descriptor table together with the working
  // directory, the root and the umask, so CLONE_FILES also shares those.
  if ((flags & CLONE_FILES) == 0) {
    if ((flags & CLONE_FS) != 0) return -1;
    rfflags |= FREEBSD_RFFDG;
  }
  // Another exit signal than SIGCHLD, or none (0).
  int sig = flags & CSIGNAL;
  if (sig != SIGCHLD) rfflags |= FREEBSD_RFTSIGZMB | (sig << FREEBSD_RFTSIGSHIFT);
  return rfflags;
}
#else
extern "C" pid_t __bionic_clone(uint32_t flags, void* child_stack, int* parent_tid, void* tls, int* child_tid, int (*fn)(void*), void* arg);
extern "C" pid_t __bionic_clone3(clone_args* cl_args, size_t cl_args_size, int (*fn)(void*),
                                 void* arg);
extern "C" __noreturn void __exit(int status);
#endif

// Called from the __bionic_clone assembler to call the thread function then exit.
__attribute__((no_sanitize("hwaddress")))
extern "C" __LIBC_HIDDEN__ void __start_thread(int (*fn)(void*), void* arg
#if defined(__FreeBSD__)
                                             , const rfork_ids* ids, int* child_tid
#endif
                                             ) {
  BIONIC_STOP_UNWIND;

  pthread_internal_t* self = __get_thread();
  if (self && self->tid == -1) {
#if defined(__FreeBSD__)
    self->tid = ids->ri_tid;
    self->set_cached_pid(ids->ri_pid);
    __main_thread = self;
#else
    self->tid = syscall(__NR_gettid);
#endif
  }

#if defined(__FreeBSD__)
  if (child_tid != nullptr) *child_tid = ids->ri_tid;
#endif
  int status = (*fn)(arg);
#if defined(__FreeBSD__)
  // The child is a process with one thread.
  _exit(status);
#else
  __exit(status);
#endif
}

struct clone_id_info {
  pthread_internal_t* self;
  pid_t parent_pid;
  pid_t caller_tid;
};

static inline clone_id_info clone_prologue(int flags) {
  // Remember the parent pid and invalidate the cached value while we clone.
  pthread_internal_t* self = __get_thread();
  pid_t parent_pid = self->invalidate_cached_pid();

  // Remmber the caller's tid so that it can be restored in the parent after clone.
  pid_t caller_tid = self->tid;
  // Invalidate the tid before the syscall. The value is lazily cached in gettid(),
  // and it will be updated by fork() and pthread_create(). We don't do this if
  // we are sharing address space with the child.
  if (!(flags & (CLONE_VM | CLONE_VFORK))) {
    self->tid = -1;
  }

#if defined(__aarch64__) && !defined(__FreeBSD__)
  // FreeBSD gives no SME to processes (no HWCAP2_SME), so ZA is always off there.
  // AAPCS64 defines SME ZA interface as private for clone(), which means that ZA state on entry
  // can be "dormant" or "off", while on return it can be unchanged or "off".
  // https://github.com/ARM-software/abi-aa/blob/main/aapcs64/aapcs64.rst#za-interfaces
  // Handling the dormant state would make the code unnecessary complex, so for simplicity turn
  // ZA state off on entry which ensures that the state on return will be "off" as well.
  __arm_za_disable();
#endif

  return {self, parent_pid, caller_tid};
}

static inline int clone_epilogue(clone_id_info& ciinfo, int clone_result
#if defined(__FreeBSD__)
                                 , const rfork_ids& ids
#endif
                                 ) {
  if (clone_result != 0) {
    // We're the parent, so put our known pid and tid back in place.
    // We leave the child without a cached pid and tid, but:
    // 1. pthread_create gives its children their own pthread_internal_t with the correct pid and
    // tid.
    // 2. fork uses CLONE_CHILD_SETTID to get the new pid/tid.
    // 3. The tid is lazily fetched in gettid().
    // If any other cases become important, we could use a double trampoline like __pthread_start.
    ciinfo.self->tid = ciinfo.caller_tid;
    ciinfo.self->set_cached_pid(ciinfo.parent_pid);
  } else if (ciinfo.self->tid == -1) {
#if defined(__FreeBSD__)
    ciinfo.self->tid = ids.ri_tid;
    ciinfo.self->set_cached_pid(ids.ri_pid);
    __main_thread = ciinfo.self;
#else
    ciinfo.self->tid = syscall(__NR_gettid);
    ciinfo.self->set_cached_pid(ciinfo.self->tid);
#endif
  }

  return clone_result;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int clone(int (*fn)(void*), void* child_stack, int flags, void* arg, ...) {
  int* parent_tid = nullptr;
  void* new_tls = nullptr;
  int* child_tid = nullptr;

  if (fn != nullptr && child_stack == nullptr) {
    errno = EINVAL;
    return -1;
  }

  // Extract any optional parameters required by the flags.
  va_list args;
  va_start(args, arg);
  if ((flags & (CLONE_PARENT_SETTID|CLONE_SETTLS|CLONE_CHILD_SETTID|CLONE_CHILD_CLEARTID)) != 0) {
    parent_tid = va_arg(args, int*);
  }
  if ((flags & (CLONE_SETTLS|CLONE_CHILD_SETTID|CLONE_CHILD_CLEARTID)) != 0) {
    new_tls = va_arg(args, void*);
  }
  if ((flags & (CLONE_CHILD_SETTID|CLONE_CHILD_CLEARTID)) != 0) {
    child_tid = va_arg(args, int*);
  }
  va_end(args);

  // Align 'child_stack' to 16 bytes.
  uintptr_t child_stack_addr = reinterpret_cast<uintptr_t>(child_stack);
  child_stack_addr &= ~0xf;
  child_stack = reinterpret_cast<void*>(child_stack_addr);

#if defined(__FreeBSD__)
  (void)parent_tid;
  (void)new_tls;
  bool use_vfork;
  int rfflags = clone_rfork_flags(flags, fn != nullptr, &use_vfork);
  if (rfflags == -1) {
    errno = EINVAL;
    return -1;
  }
#endif

  clone_id_info ciinfo = clone_prologue(flags);
#if defined(__FreeBSD__)
  rfork_ids ids;
  int* store_tid = (flags & CLONE_CHILD_SETTID) != 0 ? child_tid : nullptr;
#endif

  // Actually do the clone.
  int clone_result;
  if (fn != nullptr) {
#if defined(__FreeBSD__)
    clone_result = __freebsd_clone(rfflags, child_stack, fn, arg, use_vfork,
                                  (flags & CLONE_VM) != 0 ? nullptr : &ids, store_tid);
#else
    clone_result = __bionic_clone(flags, child_stack, parent_tid, new_tls, child_tid, fn, arg);
#endif
  } else {
#if defined(__FreeBSD__)
    clone_result = __freebsd_rfork_ids(rfflags, &ids);
#elif defined(__x86_64__) // sys_clone's last two arguments are flipped on x86-64.
    clone_result = syscall(__NR_clone, flags, child_stack, parent_tid, child_tid, new_tls);
#else
    clone_result = syscall(__NR_clone, flags, child_stack, parent_tid, new_tls, child_tid);
#endif
  }

#if defined(__FreeBSD__)
  clone_result = clone_epilogue(ciinfo, clone_result, ids);
  if (clone_result == 0 && store_tid != nullptr) *store_tid = ids.ri_tid;
  return clone_result;
#else
  return clone_epilogue(ciinfo, clone_result);
#endif
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int clone3(struct clone_args* cl_args, size_t size, int (*fn)(void*), void* arg) {
#if defined(__FreeBSD__)
  // FreeBSD has no clone3; a Linux kernel without it gives ENOSYS too.
  (void)cl_args;
  (void)size;
  (void)fn;
  (void)arg;
  errno = ENOSYS;
  return -1;
#else
  bool invalid_args = (cl_args == nullptr) || ((cl_args->flags & CLONE_VM) && fn == nullptr) ||
                      (fn == nullptr && cl_args->stack != 0) ||
                      (fn != nullptr && cl_args->stack == 0);

  if (invalid_args) {
    errno = EINVAL;
    return -1;
  }

  clone_id_info ciinfo = clone_prologue(cl_args->flags);

  int clone_result;
  if (fn != nullptr) {
    // This call does not return in the child process.
    clone_result = __bionic_clone3(cl_args, size, fn, arg);
  } else {
    clone_result = syscall(SYS_clone3, cl_args, size);
  }

  return clone_epilogue(ciinfo, clone_result);
#endif
}
