/*
 * Copyright (c) 2026 Ayeed Shaikh <Ayeed.AI10@race.reva.edu.in>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * The radio switch events of Linux, for <linux/rfkill.h>.  FreeBSD has no
 * rfkill device.  The names have the Linux values, and /dev/rfkill does
 * not exist, so a program fails when it opens the device.
 */

#pragma once

#include <stdint.h>

struct rfkill_event {
	uint32_t	idx;
	uint8_t		type;
	uint8_t		op;
	uint8_t		soft;
	uint8_t		hard;
};

#define	RFKILL_OP_CHANGE	2
#define	RFKILL_OP_CHANGE_ALL	3

#define	RFKILL_TYPE_ALL		0
#define	RFKILL_TYPE_WLAN	1
#define	RFKILL_TYPE_BLUETOOTH	2
#define	RFKILL_TYPE_UWB		3
#define	RFKILL_TYPE_WIMAX	4
#define	RFKILL_TYPE_WWAN	5
#define	RFKILL_TYPE_GPS		6
