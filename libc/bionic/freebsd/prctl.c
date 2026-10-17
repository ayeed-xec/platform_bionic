/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * prctl(2) for bionic-freebsd.  FreeBSD has no prctl(2), so libc emulates
 * each option on native system calls.  This version emulates no options:
 * all of them fail with EINVAL, as on a Linux kernel that does not have
 * the option.  The startup code uses only PR_SET_VMA (names for memory
 * maps), and it ignores the failure.  PR_SET_NAME and PR_GET_NAME
 * (thr_set_name(2)) and the other options come later in M1.
 */

#include <errno.h>
#include <sys/prctl.h>

int
prctl(int op, ...)
{
	(void)op;
	errno = EINVAL;
	return (-1);
}
