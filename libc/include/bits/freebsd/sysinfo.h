/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * struct sysinfo of sysinfo(2).  FreeBSD has no sysinfo(2): libc emulates
 * it, so the structure has the Android (LP64) layout, as in FreeBSD's
 * Linux emulation (sys/compat/linux/linux_misc.c, struct l_sysinfo).  On
 * LP64 the Linux padding at the end is empty.
 */

#pragma once

#include <sys/cdefs.h>

#define	SI_LOAD_SHIFT	16

struct sysinfo {
	long		uptime;
	unsigned long	loads[3];
	unsigned long	totalram;
	unsigned long	freeram;
	unsigned long	sharedram;
	unsigned long	bufferram;
	unsigned long	totalswap;
	unsigned long	freeswap;
	unsigned short	procs;
	unsigned short	pad;
	unsigned long	totalhigh;
	unsigned long	freehigh;
	unsigned int	mem_unit;
};
