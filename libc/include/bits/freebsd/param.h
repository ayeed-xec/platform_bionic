/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * The <sys/param.h> constants that the Linux UAPI gave.  The kernel does
 * not read them, so they keep the Android values (ADR 0002, decision 1.3).
 */

#pragma once

#define	HZ		100
#define	EXEC_PAGESIZE	4096
#define	NOGROUP		(-1)
#define	MAXHOSTNAMELEN	64
