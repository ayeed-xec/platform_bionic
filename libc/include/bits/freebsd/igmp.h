/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * IGMP message types, from FreeBSD sys/netinet/igmp.h.  They are wire
 * values (RFC 1112, RFC 2236 and RFC 3376), the same on each system.
 * <netinet/igmp.h> has struct igmp.
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
