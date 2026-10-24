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

/**
 * @file net/if_bridgevar.h
 * @brief FreeBSD's if_bridge(4) commands, which SIOCSDRVSPEC and
 * SIOCGDRVSPEC carry in a struct ifdrv.  It is a FreeBSD-only API, not part
 * of the NDK.  abi_check.c compares it with the FreeBSD header.
 */

#include <sys/types.h>
#include <sys/time.h>

#include <net/if.h>
#include <stdint.h>

#define	BRDGADD		0	/* add a member (ifbreq) */
#define	BRDGDEL		1	/* delete a member (ifbreq) */
#define	BRDGSIFFLGS	3	/* set the flags of a member (ifbreq) */
#define	BRDGGIFS	6	/* get the members (ifbifconf) */
#define	BRDGSTO		9	/* set the address cache time (ifbrparam) */
#define	BRDGSPRI	14	/* set the bridge priority (ifbrparam) */
#define	BRDGSHT		16	/* set the hello time (ifbrparam) */
#define	BRDGSFD		18	/* set the forward delay (ifbrparam) */
#define	BRDGSMA		20	/* set the maximum age (ifbrparam) */
#define	BRDGSIFPRIO	21	/* set the priority of a member (ifbreq) */
#define	BRDGSIFCOST	22	/* set the path cost of a member (ifbreq) */
#define	BRDGPARAM	25	/* get the spanning tree state (ifbropreq) */

/** The member takes part in the spanning tree. */
#define	IFBIF_STP	0x0004
/** The member is a span port, which gets a copy of each frame. */
#define	IFBIF_SPAN	0x0008

struct ifbreq {
	char		ifbr_ifsname[IFNAMSIZ];
	uint32_t	ifbr_ifsflags;
	uint32_t	ifbr_stpflags;
	uint32_t	ifbr_path_cost;
	uint8_t		ifbr_portno;
	uint8_t		ifbr_priority;
	uint8_t		ifbr_proto;
	uint8_t		ifbr_role;
	uint8_t		ifbr_state;
	uint32_t	ifbr_addrcnt;
	uint32_t	ifbr_addrmax;
	uint32_t	ifbr_addrexceeded;
	uint16_t	ifbr_pvid;
	uint16_t	ifbr_vlanproto;
	uint8_t		pad[28];
};

struct ifbifconf {
	uint32_t	ifbic_len;
	union {
		char		*ifbicu_buf;
		struct ifbreq	*ifbicu_req;
	} ifbic_ifbicu;
};
#define	ifbic_buf	ifbic_ifbicu.ifbicu_buf
#define	ifbic_req	ifbic_ifbicu.ifbicu_req

struct ifbrparam {
	union {
		uint32_t	ifbrpu_int32;
		uint16_t	ifbrpu_int16;
		uint8_t		ifbrpu_int8;
	} ifbrp_ifbrpu;
};
#define	ifbrp_ctime	ifbrp_ifbrpu.ifbrpu_int32	/* seconds */
#define	ifbrp_prio	ifbrp_ifbrpu.ifbrpu_int16
#define	ifbrp_hellotime	ifbrp_ifbrpu.ifbrpu_int8	/* seconds */
#define	ifbrp_fwddelay	ifbrp_ifbrpu.ifbrpu_int8	/* seconds */
#define	ifbrp_maxage	ifbrp_ifbrpu.ifbrpu_int8	/* seconds */

struct ifbropreq {
	uint8_t		ifbop_holdcount;
	uint8_t		ifbop_maxage;
	uint8_t		ifbop_hellotime;
	uint8_t		ifbop_fwddelay;
	uint8_t		ifbop_protocol;
	uint16_t	ifbop_priority;
	uint16_t	ifbop_root_port;
	uint32_t	ifbop_root_path_cost;
	uint64_t	ifbop_bridgeid;
	uint64_t	ifbop_designated_root;
	uint64_t	ifbop_designated_bridge;
	struct timeval	ifbop_last_tc_time;
};
