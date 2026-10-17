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

#pragma once

#include <sys/cdefs.h>

#include <errno.h>
#if defined(__FreeBSD__)
#include <bits/freebsd/umtx.h>
#include <time.h>
#else
#include <linux/futex.h>
#endif
#include <stdbool.h>
#include <stddef.h>
#include <sys/syscall.h>
#include <unistd.h>

struct timespec;

#if defined(__FreeBSD__)
// FreeBSD has no futex(2). Only libc sees these operation codes: __futex()
// does each operation with _umtx_op(2) (ADR 0002, decision 1.3).
#define FUTEX_WAIT 0
#define FUTEX_WAKE 1
#define FUTEX_LOCK_PI 6
#define FUTEX_UNLOCK_PI 7
#define FUTEX_WAIT_BITSET 9
#define FUTEX_LOCK_PI2 13
#define FUTEX_PRIVATE_FLAG 128
#define FUTEX_CLOCK_REALTIME 256
#define FUTEX_CMD_MASK (~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME))
#define FUTEX_WAIT_PRIVATE (FUTEX_WAIT | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_PRIVATE (FUTEX_WAKE | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAIT_BITSET_PRIVATE (FUTEX_WAIT_BITSET | FUTEX_PRIVATE_FLAG)
#define FUTEX_UNLOCK_PI_PRIVATE (FUTEX_UNLOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_BITSET_MATCH_ANY 0xffffffff
#define FUTEX_TID_MASK 0x3fffffff

extern "C" int __umtx_op(void* obj, int op, unsigned long val, void* uaddr, void* uaddr2);

// Returns 0, or -1 with errno set, as a system call does. FUTEX_WAIT has a
// relative timeout; FUTEX_WAIT_BITSET has an absolute timeout.
static inline __always_inline int __futex_umtx(volatile void* ftx, int op, int value,
                                               const timespec* timeout, int bitset) {
  void* obj = const_cast<void*>(ftx);
  bool is_private = (op & FUTEX_PRIVATE_FLAG) != 0;
  int wait_op = is_private ? UMTX_OP_WAIT_UINT_PRIVATE : UMTX_OP_WAIT_UINT;
  // The kernel compares the 32-bit word with val as an unsigned value.
  unsigned long val = static_cast<unsigned int>(value);

  switch (op & FUTEX_CMD_MASK) {
    case FUTEX_WAIT:
      return __umtx_op(obj, wait_op, val,
                       timeout ? reinterpret_cast<void*>(sizeof(*timeout)) : nullptr,
                       const_cast<timespec*>(timeout));
    case FUTEX_WAIT_BITSET: {
      if (bitset != static_cast<int>(FUTEX_BITSET_MATCH_ANY)) break;
      if (timeout == nullptr) return __umtx_op(obj, wait_op, val, nullptr, nullptr);
      uint32_t clock = (op & FUTEX_CLOCK_REALTIME) ? CLOCK_REALTIME : CLOCK_MONOTONIC;
      _umtx_time ut = {*timeout, UMTX_ABSTIME, clock};
      return __umtx_op(obj, wait_op, val, reinterpret_cast<void*>(sizeof(ut)), &ut);
    }
    case FUTEX_WAKE:
      return __umtx_op(obj, is_private ? UMTX_OP_WAKE_PRIVATE : UMTX_OP_WAKE, val, nullptr,
                       nullptr);
  }
  // No priority-inheritance futexes yet.
  errno = ENOSYS;
  return -1;
}
#endif

static inline __always_inline int __futex(volatile void* ftx, int op, int value,
                                          const timespec* timeout, int bitset) {
  // Our generated syscall assembler sets errno, but our callers (pthread functions) don't want to.
  int saved_errno = errno;
#if defined(__FreeBSD__)
  int result = __futex_umtx(ftx, op, value, timeout, bitset);
#else
  int result = syscall(__NR_futex, ftx, op, value, timeout, NULL, bitset);
#endif
  if (__predict_false(result == -1)) {
    result = -errno;
    errno = saved_errno;
  }
  return result;
}

static inline int __futex_wake(volatile void* ftx, int count) {
  return __futex(ftx, FUTEX_WAKE, count, nullptr, 0);
}

static inline int __futex_wake_ex(volatile void* ftx, bool shared, int count) {
  return __futex(ftx, shared ? FUTEX_WAKE : FUTEX_WAKE_PRIVATE, count, nullptr, 0);
}

static inline int __futex_wait(volatile void* ftx, int value, const timespec* timeout) {
  return __futex(ftx, FUTEX_WAIT, value, timeout, 0);
}

static inline int __futex_wait_ex(volatile void* ftx, bool shared, int value) {
  return __futex(ftx, (shared ? FUTEX_WAIT_BITSET : FUTEX_WAIT_BITSET_PRIVATE), value, nullptr,
                 FUTEX_BITSET_MATCH_ANY);
}

__LIBC_HIDDEN__ int __futex_wait_ex(volatile void* ftx, bool shared, int value,
                                    bool use_realtime_clock, const timespec* abs_timeout);

static inline int __futex_pi_unlock(volatile void* ftx, bool shared) {
  return __futex(ftx, shared ? FUTEX_UNLOCK_PI : FUTEX_UNLOCK_PI_PRIVATE, 0, nullptr, 0);
}

__LIBC_HIDDEN__ int __futex_pi_lock_ex(volatile void* ftx, bool shared, bool use_realtime_clock,
                                       const timespec* abs_timeout);
