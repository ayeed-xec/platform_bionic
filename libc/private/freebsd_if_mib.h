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

#pragma once

#include <sys/types.h>

#include <net/if.h>
#include <stdint.h>

/* FreeBSD's interface MIB.  These names do not enter the netlink ABI. */
#define	FREEBSD_NETLINK_GENERIC	0
#define	FREEBSD_IFMIB_IFDATA	2
#define	FREEBSD_IFDATA_GENERAL	1

/* Field order and widths of FreeBSD 15.1 struct if_data. */
struct freebsd_if_data {
	uint8_t		ifi_type;
	uint8_t		ifi_physical;
	uint8_t		ifi_addrlen;
	uint8_t		ifi_hdrlen;
	uint8_t		ifi_link_state;
	uint8_t		ifi_vhid;
	uint16_t	ifi_datalen;
	uint32_t	ifi_mtu;
	uint32_t	ifi_metric;
	uint64_t	ifi_baudrate;
	uint64_t	ifi_ipackets;
	uint64_t	ifi_ierrors;
	uint64_t	ifi_opackets;
	uint64_t	ifi_oerrors;
	uint64_t	ifi_collisions;
	uint64_t	ifi_ibytes;
	uint64_t	ifi_obytes;
	uint64_t	ifi_imcasts;
	uint64_t	ifi_omcasts;
	uint64_t	ifi_iqdrops;
	uint64_t	ifi_oqdrops;
	uint64_t	ifi_noproto;
	uint64_t	ifi_hwassist;
	uint64_t	ifi_epoch;
	uint64_t	ifi_lastchange[2];
};

struct freebsd_ifmibdata {
	char		ifmd_name[IFNAMSIZ];
	int32_t		ifmd_pcount;
	int32_t		ifmd_flags;
	int32_t		ifmd_snd_len;
	int32_t		ifmd_snd_maxlen;
	int32_t		ifmd_snd_drops;
	int32_t		ifmd_filler[4];
	struct freebsd_if_data ifmd_data;
};

static_assert(sizeof(struct freebsd_if_data) == 152);
static_assert(sizeof(struct freebsd_ifmibdata) == 208);
