/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "private/bionic_defs.h"
#include "private/ErrnoRestorer.h"

#if defined(__FreeBSD__)
extern "C" int __sigqueue(pid_t, int, void*);
#endif
#include "pthread_internal.h"

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_sigqueue(pthread_t t, int sig, const union sigval value) {
  ErrnoRestorer errno_restorer;

  pid_t tid = __pthread_internal_gettid(t, "pthread_sigqueue");
  if (tid == -1) return ESRCH;

#if defined(__FreeBSD__)
  // sigqueue(2) with __SIGQUEUE_TID queues the signal to a thread of this
  // process. The kernel fills in SI_QUEUE, the pid and the uid.
  return __sigqueue(tid, sig | __SIGQUEUE_TID, value.sival_ptr) ? errno : 0;
#else
  pid_t pid = getpid();

  siginfo_t siginfo = { .si_code = SI_QUEUE };
  siginfo.si_signo = sig;
  siginfo.si_pid = pid;
  siginfo.si_uid = getuid();
  siginfo.si_value = value;

  return syscall(__NR_rt_tgsigqueueinfo, pid, tid, sig, &siginfo) ? errno : 0;
#endif
}
