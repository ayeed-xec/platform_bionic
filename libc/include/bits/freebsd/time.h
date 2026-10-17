/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
