/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * wait4(2) and wait6(2) options and ID types of the FreeBSD kernel ABI,
 * from FreeBSD sys/sys/wait.h.  The kernel reads them, so they have the
 * FreeBSD values (ADR 0002, decision 1.3).  abi_check.c checks them.
 */

#pragma once

#define	WNOHANG		1
#define	WUNTRACED	2
#define	WSTOPPED	WUNTRACED
#define	WCONTINUED	4
#define	WNOWAIT		8
#define	WEXITED		16
#define	WTRAPPED	32

#define	P_PID		0
#define	P_PGID		2
#define	P_ALL		7