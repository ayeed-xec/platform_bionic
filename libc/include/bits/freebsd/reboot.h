/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * reboot(2) values of the FreeBSD kernel ABI, from FreeBSD sys/sys/reboot.h,
 * with the names of <sys/reboot.h>.  The kernel reads them, so they have
 * the FreeBSD values.  RB_POWER_OFF also has RB_HALT: if the power cannot
 * go off, the system halts, as on Linux.  FreeBSD reboot(2) has no
 * Ctrl-Alt-Del switch, so there is no RB_ENABLE_CAD or RB_DISABLE_CAD.
 * abi_check.c checks the values.
 */

#pragma once

#define	RB_AUTOBOOT	0
#define	RB_HALT_SYSTEM	0x008
#define	RB_POWER_OFF	(RB_HALT_SYSTEM | 0x4000)
