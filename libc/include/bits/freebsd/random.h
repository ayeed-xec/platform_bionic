/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * getrandom(2) flags of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/random.h.  They have the same values as on Linux.
 * abi_check.c checks them.
 */

#pragma once

#define	GRND_NONBLOCK	0x1
#define	GRND_RANDOM	0x2
#define	GRND_INSECURE	0x4
