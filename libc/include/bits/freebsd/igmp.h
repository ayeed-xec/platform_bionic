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
 * IGMP message types from FreeBSD's <netinet/igmp.h>.  These are wire
 * values (RFC 1112, 2236 and 3376), so every system uses the same numbers.
 * struct igmp is in <netinet/igmp.h>.
 */

#pragma once

#define	IGMP_MINLEN			8

#define	IGMP_HOST_MEMBERSHIP_QUERY	0x11
#define	IGMP_v1_HOST_MEMBERSHIP_REPORT	0x12
#define	IGMP_DVMRP			0x13
#define	IGMP_PIM			0x14
#define	IGMP_v2_HOST_MEMBERSHIP_REPORT	0x16
#define	IGMP_HOST_LEAVE_MESSAGE		0x17
#define	IGMP_MTRACE_REPLY		0x1e
#define	IGMP_MTRACE_QUERY		0x1f
#define	IGMP_v3_HOST_MEMBERSHIP_REPORT	0x22
