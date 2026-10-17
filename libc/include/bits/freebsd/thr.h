/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * The thr_new(2) parameter block of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/thr.h.  pthread_create() uses it.  abi_check.c checks it.
 */

#pragma once

#include <sys/cdefs.h>
#include <stddef.h>

struct rtprio;

struct thr_param {
	void		(*start_func)(void *);
	void		*arg;
	char		*stack_base;
	size_t		stack_size;
	char		*tls_base;
	size_t		tls_size;
	long		*child_tid;
	long		*parent_tid;
	int		flags;
	struct rtprio	*rtp;
	void		*spare[3];
};
