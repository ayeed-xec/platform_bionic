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
 * @file netinet6/in6_var.h
 * @brief FreeBSD's ioctls that add and delete an IPv6 address of an
 * interface, on an AF_INET6 socket.  It is a FreeBSD-only API, not part of
 * the NDK.  abi_check.c compares it with the FreeBSD headers.
 */

#include <sys/types.h>

#include <net/if.h>
#include <netinet/in.h>
#include <stdint.h>
#include <time.h>

#include <bits/freebsd/ioccom.h>

struct in6_addrlifetime {
	time_t		ia6t_expire;
	time_t		ia6t_preferred;
	uint32_t	ia6t_vltime;
	uint32_t	ia6t_pltime;
};

/*
 * The union of FreeBSD has more members, which Android code does not use.
 * __ifru_size gives the union its size and alignment.
 */
struct in6_ifreq {
	char		ifr_name[IFNAMSIZ];
	union {
		struct sockaddr_in6	ifru_addr;
		uint64_t		__ifru_size[34];
	} ifr_ifru;
};

struct in6_aliasreq {
	char			ifra_name[IFNAMSIZ];
	struct sockaddr_in6	ifra_addr;
	struct sockaddr_in6	ifra_dstaddr;
	struct sockaddr_in6	ifra_prefixmask;
	int			ifra_flags;
	struct in6_addrlifetime	ifra_lifetime;
	int			ifra_vhid;
};

#define	SIOCDIFADDR_IN6	_IOW('i', 25, struct in6_ifreq)
#define	SIOCAIFADDR_IN6	_IOW('i', 27, struct in6_aliasreq)

/** A lifetime that does not end (ND6_INFINITE_LIFETIME of <netinet6/nd6.h>). */
#define	ND6_INFINITE_LIFETIME	0xffffffff
