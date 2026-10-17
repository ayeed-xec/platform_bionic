/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Resource limits and usage of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/resource.h.  RLIM_INFINITY is the largest FreeBSD rlim_t.  rlim_t
 * keeps the Android type: it has the same size as the FreeBSD type, the
 * kernel returns no negative limits, and it reads a negative limit as
 * RLIM_INFINITY.  FreeBSD has no RLIMIT_LOCKS, RLIMIT_SIGPENDING,
 * RLIMIT_MSGQUEUE, RLIMIT_NICE, RLIMIT_RTPRIO or RLIMIT_RTTIME.
 * abi_check.c checks the values.
 */

#pragma once

#include <sys/cdefs.h>
#include <stdint.h>
#include <bits/freebsd/time.h>

typedef unsigned long		rlim_t;
typedef unsigned long long	rlim64_t;

#define	PRIO_MIN	-20
#define	PRIO_MAX	20
#define	PRIO_PROCESS	0
#define	PRIO_PGRP	1
#define	PRIO_USER	2

#define	RUSAGE_SELF	0
#define	RUSAGE_CHILDREN	-1
#define	RUSAGE_THREAD	1

struct rusage {
	struct timeval	ru_utime;
	struct timeval	ru_stime;
	long		ru_maxrss;
	long		ru_ixrss;
	long		ru_idrss;
	long		ru_isrss;
	long		ru_minflt;
	long		ru_majflt;
	long		ru_nswap;
	long		ru_inblock;
	long		ru_oublock;
	long		ru_msgsnd;
	long		ru_msgrcv;
	long		ru_nsignals;
	long		ru_nvcsw;
	long		ru_nivcsw;
};

#define	RLIMIT_CPU	0
#define	RLIMIT_FSIZE	1
#define	RLIMIT_DATA	2
#define	RLIMIT_STACK	3
#define	RLIMIT_CORE	4
#define	RLIMIT_RSS	5
#define	RLIMIT_MEMLOCK	6
#define	RLIMIT_NPROC	7
#define	RLIMIT_NOFILE	8
#define	RLIMIT_SBSIZE	9
#define	RLIMIT_VMEM	10
#define	RLIMIT_AS	RLIMIT_VMEM
#define	RLIMIT_NPTS	11
#define	RLIMIT_SWAP	12
#define	RLIMIT_KQUEUES	13
#define	RLIMIT_UMTXP	14
#define	RLIMIT_PIPEBUF	15
#define	RLIM_NLIMITS	16

#define	RLIM_INFINITY	((rlim_t)0x7fffffffffffffff)
#define	RLIM64_INFINITY	((rlim64_t)RLIM_INFINITY)

struct rlimit {
	rlim_t		rlim_cur;
	rlim_t		rlim_max;
};

struct rlimit64 {
	rlim64_t	rlim_cur;
	rlim64_t	rlim_max;
};
