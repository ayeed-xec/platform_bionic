/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * The _umtx_op(2) operations that bionic's futex code uses, and the
 * timeout structure, from FreeBSD sys/sys/umtx.h and sys/sys/_umtx.h.
 * abi_check.c checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <stdint.h>
#include <bits/timespec.h>

#define	UMTX_OP_WAIT			2
#define	UMTX_OP_WAKE			3
#define	UMTX_OP_WAIT_UINT		11
#define	UMTX_OP_WAIT_UINT_PRIVATE	15
#define	UMTX_OP_WAKE_PRIVATE		16

#define	UMTX_ABSTIME			0x01

struct _umtx_time {
	struct timespec	_timeout;
	uint32_t	_flags;
	uint32_t	_clockid;
};
