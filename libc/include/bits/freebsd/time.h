/*
 * Copyright (c) 2026 Ayeed Shaikh <Ayeed.AI10@race.reva.edu.in>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
 * Time structures and clock IDs of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/_clock_id.h, sys/sys/_timeval.h and sys/sys/time.h.  The Linux
 * clock IDs 1, 7, 8 and 9 do not exist on FreeBSD (probe P5).
 * abi_check.c checks the values.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>
#include <bits/timespec.h>

struct timeval {
	time_t		tv_sec;
	suseconds_t	tv_usec;
};

struct timezone {
	int		tz_minuteswest;
	int		tz_dsttime;
};

struct itimerval {
	struct timeval	it_interval;
	struct timeval	it_value;
};

struct itimerspec {
	struct timespec	it_interval;
	struct timespec	it_value;
};

#define	ITIMER_REAL		0
#define	ITIMER_VIRTUAL		1
#define	ITIMER_PROF		2

#define	CLOCK_REALTIME		0
#define	CLOCK_VIRTUAL		1
#define	CLOCK_PROF		2
#define	CLOCK_MONOTONIC		4
#define	CLOCK_UPTIME		5
#define	CLOCK_UPTIME_PRECISE	7
#define	CLOCK_UPTIME_FAST	8
#define	CLOCK_REALTIME_PRECISE	9
#define	CLOCK_REALTIME_FAST	10
#define	CLOCK_MONOTONIC_PRECISE	11
#define	CLOCK_MONOTONIC_FAST	12
#define	CLOCK_SECOND		13
#define	CLOCK_THREAD_CPUTIME_ID	14
#define	CLOCK_PROCESS_CPUTIME_ID 15
#define	CLOCK_TAI		16
/* Linux names that FreeBSD defines. */
#define	CLOCK_BOOTTIME		CLOCK_MONOTONIC
#define	CLOCK_REALTIME_COARSE	CLOCK_REALTIME_FAST
#define	CLOCK_MONOTONIC_COARSE	CLOCK_MONOTONIC_FAST

#define	TIMER_RELTIME		0x0
#define	TIMER_ABSTIME		0x1

/* The "which" argument of clock_getcpuclockid2(2). */
#define	CPUCLOCK_WHICH_PID	0
#define	CPUCLOCK_WHICH_TID	1
