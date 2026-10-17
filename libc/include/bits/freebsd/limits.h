/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Limits that the FreeBSD kernel enforces, from FreeBSD
 * sys/sys/syslimits.h.  The kernel rejects a longer path with
 * ENAMETOOLONG, so PATH_MAX is 1024, not the Linux 4096.
 * abi_check.c checks the values.
 */

#pragma once

#define	ARG_MAX		(2 * 256 * 1024)
#define	CHILD_MAX	40
#define	MAX_CANON	255
#define	MAX_INPUT	255
#define	NAME_MAX	255
#define	NGROUPS_MAX	1023
#define	OPEN_MAX	64
#define	PATH_MAX	1024
#define	PIPE_BUF	512
