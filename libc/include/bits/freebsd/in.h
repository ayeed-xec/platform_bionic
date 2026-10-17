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
 * Internet protocol numbers, IPv4 socket options and address macros, and the
 * IPv4 structures of the FreeBSD kernel ABI, from FreeBSD sys/netinet/in.h.
 * The FreeBSD kernel reads these values and layouts, so they are the FreeBSD
 * values.  Names that only Linux has are not defined.  The IPv6 names are in
 * bits/freebsd/in6.h.  abi_check.c checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <stdint.h>
#include <bits/in_addr.h>
#include <bits/sa_family_t.h>
#include <bits/sockaddr_storage.h>

typedef uint16_t in_port_t;

/* Protocol numbers for the level argument of setsockopt(2) and for sockets. */
#define	IPPROTO_IP		0
#define	IPPROTO_HOPOPTS		0
#define	IPPROTO_ICMP		1
#define	IPPROTO_IGMP		2
#define	IPPROTO_GGP		3
#define	IPPROTO_IPV4		4
#define	IPPROTO_IPIP		IPPROTO_IPV4
#define	IPPROTO_TCP		6
#define	IPPROTO_ST		7
#define	IPPROTO_EGP		8
#define	IPPROTO_PIGP		9
#define	IPPROTO_RCCMON		10
#define	IPPROTO_NVPII		11
#define	IPPROTO_PUP		12
#define	IPPROTO_ARGUS		13
#define	IPPROTO_EMCON		14
#define	IPPROTO_XNET		15
#define	IPPROTO_CHAOS		16
#define	IPPROTO_UDP		17
#define	IPPROTO_MUX		18
#define	IPPROTO_MEAS		19
#define	IPPROTO_HMP		20
#define	IPPROTO_PRM		21
#define	IPPROTO_IDP		22
#define	IPPROTO_TRUNK1		23
#define	IPPROTO_TRUNK2		24
#define	IPPROTO_LEAF1		25
#define	IPPROTO_LEAF2		26
#define	IPPROTO_RDP		27
#define	IPPROTO_IRTP		28
#define	IPPROTO_TP		29
#define	IPPROTO_BLT		30
#define	IPPROTO_NSP		31
#define	IPPROTO_INP		32
#define	IPPROTO_DCCP		33
#define	IPPROTO_3PC		34
#define	IPPROTO_IDPR		35
#define	IPPROTO_XTP		36
#define	IPPROTO_DDP		37
#define	IPPROTO_CMTP		38
#define	IPPROTO_TPXX		39
#define	IPPROTO_IL		40
#define	IPPROTO_IPV6		41
#define	IPPROTO_SDRP		42
#define	IPPROTO_ROUTING		43
#define	IPPROTO_FRAGMENT	44
#define	IPPROTO_IDRP		45
#define	IPPROTO_RSVP		46
#define	IPPROTO_GRE		47
#define	IPPROTO_MHRP		48
#define	IPPROTO_BHA		49
#define	IPPROTO_ESP		50
#define	IPPROTO_AH		51
#define	IPPROTO_INLSP		52
#define	IPPROTO_SWIPE		53
#define	IPPROTO_NHRP		54
#define	IPPROTO_MOBILE		55
#define	IPPROTO_TLSP		56
#define	IPPROTO_SKIP		57
#define	IPPROTO_ICMPV6		58
#define	IPPROTO_NONE		59
#define	IPPROTO_DSTOPTS		60
#define	IPPROTO_AHIP		61
#define	IPPROTO_CFTP		62
#define	IPPROTO_HELLO		63
#define	IPPROTO_SATEXPAK	64
#define	IPPROTO_KRYPTOLAN	65
#define	IPPROTO_RVD		66
#define	IPPROTO_IPPC		67
#define	IPPROTO_ADFS		68
#define	IPPROTO_SATMON		69
#define	IPPROTO_VISA		70
#define	IPPROTO_IPCV		71
#define	IPPROTO_CPNX		72
#define	IPPROTO_CPHB		73
#define	IPPROTO_WSN		74
#define	IPPROTO_PVP		75
#define	IPPROTO_BRSATMON	76
#define	IPPROTO_ND		77
#define	IPPROTO_WBMON		78
#define	IPPROTO_WBEXPAK		79
#define	IPPROTO_EON		80
#define	IPPROTO_VMTP		81
#define	IPPROTO_SVMTP		82
#define	IPPROTO_VINES		83
#define	IPPROTO_TTP		84
#define	IPPROTO_IGP		85
#define	IPPROTO_DGP		86
#define	IPPROTO_TCF		87
#define	IPPROTO_IGRP		88
#define	IPPROTO_OSPFIGP		89
#define	IPPROTO_SRPC		90
#define	IPPROTO_LARP		91
#define	IPPROTO_MTP		92
#define	IPPROTO_AX25		93
#define	IPPROTO_IPEIP		94
#define	IPPROTO_MICP		95
#define	IPPROTO_SCCSP		96
#define	IPPROTO_ETHERIP		97
#define	IPPROTO_ENCAP		98
#define	IPPROTO_APES		99
#define	IPPROTO_GMTP		100
#define	IPPROTO_PIM		103
#define	IPPROTO_IPCOMP		108
#define	IPPROTO_CARP		112
#define	IPPROTO_PGM		113
#define	IPPROTO_SCTP		132
#define	IPPROTO_MH		135
#define	IPPROTO_UDPLITE		136
#define	IPPROTO_MPLS		137
#define	IPPROTO_HIP		139
#define	IPPROTO_SHIM6		140
#define	IPPROTO_PFSYNC		240
#define	IPPROTO_RAW		255
#define	IPPROTO_MAX		256
#define	IPPROTO_DIVERT		258

/* Option names at the IPPROTO_IP level. */
#define	IP_OPTIONS		1
#define	IP_HDRINCL		2
#define	IP_TOS			3
#define	IP_TTL			4
#define	IP_RECVOPTS		5
#define	IP_RECVRETOPTS		6
#define	IP_RECVDSTADDR		7
#define	IP_SENDSRCADDR		IP_RECVDSTADDR
#define	IP_RETOPTS		8
#define	IP_MULTICAST_IF		9
#define	IP_MULTICAST_TTL	10
#define	IP_MULTICAST_LOOP	11
#define	IP_ADD_MEMBERSHIP	12
#define	IP_DROP_MEMBERSHIP	13
#define	IP_PORTRANGE		19
#define	IP_RECVIF		20
#define	IP_IPSEC_POLICY		21
#define	IP_ONESBCAST		23
#define	IP_BINDANY		24
#define	IP_ORIGDSTADDR		27
#define	IP_RECVTTL		65
#define	IP_MINTTL		66
#define	IP_DONTFRAG		67
#define	IP_RECVTOS		68
#define	IP_ADD_SOURCE_MEMBERSHIP	70
#define	IP_DROP_SOURCE_MEMBERSHIP	71
#define	IP_BLOCK_SOURCE		72
#define	IP_UNBLOCK_SOURCE	73
#define	IP_MSFILTER		74

/* Default values and the values of the IP_PORTRANGE option. */
#define	IP_DEFAULT_MULTICAST_TTL	1
#define	IP_DEFAULT_MULTICAST_LOOP	1
#define	IP_PORTRANGE_DEFAULT	0
#define	IP_PORTRANGE_HIGH	1
#define	IP_PORTRANGE_LOW	2

/* Option names for the protocol independent multicast calls (RFC 3678). */
#define	MCAST_JOIN_GROUP	80
#define	MCAST_LEAVE_GROUP	81
#define	MCAST_JOIN_SOURCE_GROUP	82
#define	MCAST_LEAVE_SOURCE_GROUP	83
#define	MCAST_BLOCK_SOURCE	84
#define	MCAST_UNBLOCK_SOURCE	85

/* The filter modes of a multicast source filter. */
#define	MCAST_INCLUDE		1
#define	MCAST_EXCLUDE		2

#define	INET_ADDRSTRLEN		16

/* The IPv4 addresses with a special meaning, in host byte order. */
#define	INADDR_ANY		((in_addr_t)0x00000000)
#define	INADDR_BROADCAST	((in_addr_t)0xffffffff)
#define	INADDR_NONE		((in_addr_t)0xffffffff)
#define	INADDR_LOOPBACK		((in_addr_t)0x7f000001)
#define	INADDR_UNSPEC_GROUP	((in_addr_t)0xe0000000)
#define	INADDR_ALLHOSTS_GROUP	((in_addr_t)0xe0000001)
#define	INADDR_ALLRTRS_GROUP	((in_addr_t)0xe0000002)

/* The classes of IPv4 address.  The argument is in host byte order. */
#define	IN_CLASSA(i)		(((uint32_t)(i) & 0x80000000) == 0)
#define	IN_CLASSB(i)		(((uint32_t)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSC(i)		(((uint32_t)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSD(i)		(((uint32_t)(i) & 0xf0000000) == 0xe0000000)
#define	IN_MULTICAST(i)		IN_CLASSD(i)
#define	IN_EXPERIMENTAL(i)	(((uint32_t)(i) & 0xf0000000) == 0xf0000000)
#define	IN_BADCLASS(i)		IN_EXPERIMENTAL(i)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff
#define	IN_CLASSD_NET		0xf0000000
#define	IN_CLASSD_NSHIFT	28
#define	IN_CLASSD_HOST		0x0fffffff
#define	IN_LOOPBACKNET		127

#define	IN_LINKLOCAL(i)		(((uint32_t)(i) & 0xffff0000) == 0xa9fe0000)
#define	IN_LOOPBACK(i)		(((uint32_t)(i) & 0xff000000) == 0x7f000000)
#define	IN_ZERONET(i)		(((uint32_t)(i) & 0xff000000) == 0)
#define	IN_LOCAL_GROUP(i)	(((uint32_t)(i) & 0xffffff00) == 0xe0000000)
#define	IN_PRIVATE(i) \
	((((uint32_t)(i) & 0xff000000) == 0x0a000000) || \
	    (((uint32_t)(i) & 0xfff00000) == 0xac100000) || \
	    (((uint32_t)(i) & 0xffff0000) == 0xc0a80000))

struct sockaddr_in {
	uint8_t		sin_len;
	sa_family_t	sin_family;
	in_port_t	sin_port;
	struct in_addr	sin_addr;
	char		sin_zero[8];
};

/* The argument of IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP. */
struct ip_mreq {
	struct in_addr	imr_multiaddr;
	struct in_addr	imr_interface;
};

/* The argument of IP_MULTICAST_IF, and of IP_ADD_MEMBERSHIP with an index. */
struct ip_mreqn {
	struct in_addr	imr_multiaddr;
	struct in_addr	imr_address;
	int		imr_ifindex;
};

/*
 * The argument of IP_ADD_SOURCE_MEMBERSHIP, IP_DROP_SOURCE_MEMBERSHIP,
 * IP_BLOCK_SOURCE and IP_UNBLOCK_SOURCE.  The order of the members is the
 * order of FreeBSD.
 */
struct ip_mreq_source {
	struct in_addr	imr_multiaddr;
	struct in_addr	imr_sourceaddr;
	struct in_addr	imr_interface;
};

/* The arguments of the MCAST_* options. */
struct group_req {
	uint32_t		gr_interface;
	struct sockaddr_storage	gr_group;
};

struct group_source_req {
	uint32_t		gsr_interface;
	struct sockaddr_storage	gsr_group;
	struct sockaddr_storage	gsr_source;
};

/* The argument of IP_MSFILTER and IPV6_MSFILTER. */
struct __msfilterreq {
	uint32_t		msfr_ifindex;
	uint32_t		msfr_fmode;
	uint32_t		msfr_nsrcs;
	struct sockaddr_storage	msfr_group;
	struct sockaddr_storage	*msfr_srcs;
};
