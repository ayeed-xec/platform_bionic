/*
 * Copyright (c) 2012 Konstantin Belousov <kib@FreeBSD.org>
 * Copyright (c) 2015 The FreeBSD Foundation
 * Copyright (c) 2026 Ayeed Shaikh <Ayeed.AI10@race.reva.edu.in>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Portions of this software were developed by Konstantin Belousov
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * The time of day without a system call.  The FreeBSD kernel keeps the state
 * of its timecounter in the timekeep page of the shared page (AT_TIMEKEEP).
 * The time is that state plus the scaled count of the counter register since
 * the kernel wrote it, as in FreeBSD libc (lib/libsys/__vdso_gettimeofday.c
 * and lib/libsys/aarch64/__vdso_gettc.c).  The kernel writes one of several
 * timehands and then makes it current; a reader starts again when the
 * generation of the timehands changed under it.
 *
 * Each function returns ENOSYS when the page cannot give the time (a clock
 * that it does not have, another counter, or the page turned off), and the
 * caller then makes the system call.
 */

#include <sys/cdefs.h>
#include <sys/time.h>

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

/* The layout of FreeBSD <sys/vdso.h> and <machine/vdso.h> on arm64. */
struct vdso_bintime {
	int64_t		sec;
	uint64_t	frac;
};

struct vdso_timehands {
	uint32_t	th_algo;
	uint32_t	th_gen;
	uint64_t	th_scale;
	uint32_t	th_offset_count;
	uint32_t	th_counter_mask;
	struct vdso_bintime th_offset;
	struct vdso_bintime th_boottime;
	uint32_t	th_physical;
	uint32_t	th_res[7];
};

struct vdso_timekeep {
	uint32_t	tk_ver;
	uint32_t	tk_enabled;
	uint32_t	tk_current;
	struct vdso_timehands tk_th[];
};

#define	VDSO_TK_VER_CURR	1
#define	VDSO_TH_ALGO_ARM_GENTIM	1

__LIBC_HIDDEN__ int	__freebsd_timekeep_clock_gettime(const void *, clockid_t,
			    struct timespec *);
__LIBC_HIDDEN__ int	__freebsd_timekeep_gettimeofday(const void *,
			    struct timeval *);

static inline int
fls32(uint32_t x)
{
	return (x == 0 ? 0 : 32 - __builtin_clz(x));
}

static inline int
fls64(uint64_t x)
{
	return (x == 0 ? 0 : 64 - __builtin_clzll(x));
}

/* Add a fraction of a second, with the carry into the seconds. */
static inline void
bt_addfrac(struct vdso_bintime *bt, uint64_t frac)
{
	uint64_t old;

	old = bt->frac;
	bt->frac += frac;
	if (bt->frac < old)
		bt->sec++;
}

static int
tc_read(const struct vdso_timehands *th, uint32_t *tc)
{
	uint64_t reg;

	if (th->th_algo != VDSO_TH_ALGO_ARM_GENTIM)
		return (ENOSYS);
	__asm __volatile("isb" : : : "memory");
	if (th->th_physical == 0)
		__asm __volatile("mrs %0, cntvct_el0" : "=r" (reg));
	else
		__asm __volatile("mrs %0, cntpct_el0" : "=r" (reg));
	*tc = (uint32_t)reg;
	return (0);
}

/* The uptime, or with "abs" the time since the Epoch, at full precision. */
static int
binuptime(const struct vdso_timekeep *tk, bool abs, struct vdso_bintime *bt)
{
	const struct vdso_timehands *th;
	uint64_t scale, x;
	uint32_t curr, delta, gen, tc;
	int error;

	do {
		if (__atomic_load_n(&tk->tk_enabled, __ATOMIC_RELAXED) == 0)
			return (ENOSYS);
		curr = __atomic_load_n(&tk->tk_current, __ATOMIC_ACQUIRE);
		th = &tk->tk_th[curr];
		gen = __atomic_load_n(&th->th_gen, __ATOMIC_ACQUIRE);
		*bt = th->th_offset;
		error = tc_read(th, &tc);
		if (error != 0)
			return (error);
		delta = (tc - th->th_offset_count) & th->th_counter_mask;
		scale = th->th_scale;
		/* The 64-bit product can overflow after a long delta. */
		if (__predict_false(fls64(scale) + fls32(delta) > 63)) {
			x = (scale >> 32) * delta;
			scale &= 0xffffffff;
			bt->sec += x >> 32;
			bt_addfrac(bt, x << 32);
		}
		bt_addfrac(bt, scale * delta);
		if (abs) {
			bt->sec += th->th_boottime.sec;
			bt_addfrac(bt, th->th_boottime.frac);
		}
		/* Read th_offset and the rest before th_gen again. */
		__atomic_thread_fence(__ATOMIC_ACQUIRE);
	} while (curr != __atomic_load_n(&tk->tk_current, __ATOMIC_RELAXED) ||
	    gen == 0 || gen != __atomic_load_n(&th->th_gen, __ATOMIC_RELAXED));
	return (0);
}

/* The uptime at the last tick of the timecounter, for the fast clocks. */
static int
getnanouptime(const struct vdso_timekeep *tk, struct vdso_bintime *bt)
{
	const struct vdso_timehands *th;
	uint32_t curr, gen;

	do {
		if (__atomic_load_n(&tk->tk_enabled, __ATOMIC_RELAXED) == 0)
			return (ENOSYS);
		curr = __atomic_load_n(&tk->tk_current, __ATOMIC_ACQUIRE);
		th = &tk->tk_th[curr];
		gen = __atomic_load_n(&th->th_gen, __ATOMIC_ACQUIRE);
		*bt = th->th_offset;
		__atomic_thread_fence(__ATOMIC_ACQUIRE);
	} while (curr != __atomic_load_n(&tk->tk_current, __ATOMIC_RELAXED) ||
	    gen == 0 || gen != __atomic_load_n(&th->th_gen, __ATOMIC_RELAXED));
	return (0);
}

int
__freebsd_timekeep_clock_gettime(const void *page, clockid_t clock_id,
    struct timespec *ts)
{
	const struct vdso_timekeep *tk;
	struct vdso_bintime bt;
	int error;

	tk = page;
	if (tk->tk_ver != VDSO_TK_VER_CURR)
		return (ENOSYS);
	switch (clock_id) {
	case CLOCK_REALTIME:
	case CLOCK_REALTIME_PRECISE:
	case CLOCK_REALTIME_FAST:
	case CLOCK_SECOND:
		error = binuptime(tk, true, &bt);
		break;
	case CLOCK_MONOTONIC:
	case CLOCK_MONOTONIC_PRECISE:
	case CLOCK_UPTIME:
	case CLOCK_UPTIME_PRECISE:
		error = binuptime(tk, false, &bt);
		break;
	case CLOCK_MONOTONIC_FAST:
	case CLOCK_UPTIME_FAST:
		error = getnanouptime(tk, &bt);
		break;
	default:
		error = ENOSYS;
		break;
	}
	if (error != 0)
		return (error);
	/* The conversion of the kernel, so that both give the same value. */
	ts->tv_sec = bt.sec;
	ts->tv_nsec = clock_id == CLOCK_SECOND ? 0 :
	    (long)(((uint64_t)1000000000 * (uint32_t)(bt.frac >> 32)) >> 32);
	return (0);
}

int
__freebsd_timekeep_gettimeofday(const void *page, struct timeval *tv)
{
	const struct vdso_timekeep *tk;
	struct vdso_bintime bt;
	int error;

	tk = page;
	if (tk->tk_ver != VDSO_TK_VER_CURR)
		return (ENOSYS);
	error = binuptime(tk, true, &bt);
	if (error != 0)
		return (error);
	tv->tv_sec = bt.sec;
	tv->tv_usec = (suseconds_t)(((uint64_t)1000000 *
	    (uint32_t)(bt.frac >> 32)) >> 32);
	return (0);
}
