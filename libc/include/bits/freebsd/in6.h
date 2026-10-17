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
 * IPv6 address, socket address, socket options and structures of the FreeBSD
 * kernel ABI, from FreeBSD sys/netinet6/in6.h.  The FreeBSD kernel reads
 * these values and layouts, so they are the FreeBSD values.  The members
 * s6_addr16 and s6_addr32 are not in the FreeBSD user header, and bionic
 * keeps them: they name the members of the same union.  Names that only
 * Linux has are not defined.  abi_check.c checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <stdint.h>
#include <bits/sa_family_t.h>
#include <bits/freebsd/in.h>

struct in6_addr {
	union {
		uint8_t		__u6_addr8[16];
		uint16_t	__u6_addr16[8];
		uint32_t	__u6_addr32[4];
	} __u6_addr;
};

#define	s6_addr			__u6_addr.__u6_addr8
#define	s6_addr16		__u6_addr.__u6_addr16
#define	s6_addr32		__u6_addr.__u6_addr32

struct sockaddr_in6 {
	uint8_t		sin6_len;
	sa_family_t	sin6_family;
	in_port_t	sin6_port;
	uint32_t	sin6_flowinfo;
	struct in6_addr	sin6_addr;
	uint32_t	sin6_scope_id;
};

#define	INET6_ADDRSTRLEN	46

/* Option names at the IPPROTO_IPV6 level. */
#define	IPV6_UNICAST_HOPS	4
#define	IPV6_MULTICAST_IF	9
#define	IPV6_MULTICAST_HOPS	10
#define	IPV6_MULTICAST_LOOP	11
#define	IPV6_JOIN_GROUP		12
#define	IPV6_LEAVE_GROUP	13
#define	IPV6_PORTRANGE		14
#define	IPV6_CHECKSUM		26
#define	IPV6_V6ONLY		27
#define	IPV6_IPSEC_POLICY	28
#define	IPV6_RTHDRDSTOPTS	35
#define	IPV6_RECVPKTINFO	36
#define	IPV6_RECVHOPLIMIT	37
#define	IPV6_RECVRTHDR		38
#define	IPV6_RECVHOPOPTS	39
#define	IPV6_RECVDSTOPTS	40
#define	IPV6_USE_MIN_MTU	42
#define	IPV6_RECVPATHMTU	43
#define	IPV6_PATHMTU		44
#define	IPV6_PKTINFO		46
#define	IPV6_HOPLIMIT		47
#define	IPV6_NEXTHOP		48
#define	IPV6_HOPOPTS		49
#define	IPV6_DSTOPTS		50
#define	IPV6_RTHDR		51
#define	IPV6_RECVTCLASS		57
#define	IPV6_AUTOFLOWLABEL	59
#define	IPV6_TCLASS		61
#define	IPV6_DONTFRAG		62
#define	IPV6_PREFER_TEMPADDR	63
#define	IPV6_BINDANY		64
#define	IPV6_ORIGDSTADDR	72
#define	IPV6_MSFILTER		74

/* Default values and the values of some options. */
#define	IPV6_DEFAULT_MULTICAST_HOPS	1
#define	IPV6_DEFAULT_MULTICAST_LOOP	1
#define	IPV6_PORTRANGE_DEFAULT	0
#define	IPV6_PORTRANGE_HIGH	1
#define	IPV6_PORTRANGE_LOW	2
#define	IPV6_RTHDR_LOOSE	0
#define	IPV6_RTHDR_STRICT	1
#define	IPV6_RTHDR_TYPE_0	0

/* The argument of IPV6_JOIN_GROUP and IPV6_LEAVE_GROUP. */
struct ipv6_mreq {
	struct in6_addr	ipv6mr_multiaddr;
	unsigned int	ipv6mr_interface;
};

/* The argument of IPV6_PKTINFO and the data of its control message. */
struct in6_pktinfo {
	struct in6_addr	ipi6_addr;
	unsigned int	ipi6_ifindex;
};

/* The data of the IPV6_PATHMTU control message. */
struct ip6_mtuinfo {
	struct sockaddr_in6	ip6m_addr;
	uint32_t		ip6m_mtu;
};
