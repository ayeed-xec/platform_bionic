/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>

#include "errno_table.inc"

long	__set_errno_from_freebsd(int) __LIBC_HIDDEN__;

/*
 * The system call stubs branch here when the kernel returns with the
 * carry flag set.  The kernel puts a positive FreeBSD errno in x0.  Set
 * errno to the Android number (docs/specs/errno-policy.md) and return -1.
 */
long
__set_errno_from_freebsd(int error)
{
	if (error > 0 && (unsigned int)error <
	    sizeof(freebsd_to_android_errno) /
	    sizeof(freebsd_to_android_errno[0]) &&
	    freebsd_to_android_errno[error] != 0)
		errno = freebsd_to_android_errno[error];
	else
		errno = EINVAL;	/* An errno that this table does not know. */
	return (-1);
}
