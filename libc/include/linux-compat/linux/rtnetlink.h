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
 * NETLINK_ROUTE link and address messages for <linux/rtnetlink.h>, taken
 * from FreeBSD's <netlink/route/common.h>, ifaddrs.h, interface.h and
 * route.h, in the order of <netlink/netlink_route.h>.  Only the parts that
 * bionic uses are here.  Those headers carry the license above and this
 * notice:
 *
 * Copyright (c) 2022 Alexander V. Chernikov <melifaro@FreeBSD.org>
 */

#pragma once

#include <sys/types.h>

#include <linux/netlink.h>

enum {
	NL_RTM_NEWLINK		= 16,
	NL_RTM_GETLINK		= 18,
	NL_RTM_NEWADDR		= 20,
	NL_RTM_GETADDR		= 22,
};

/* The kernel keeps RTM_* for route(4), so only userland has these names. */
#define	RTM_NEWLINK		NL_RTM_NEWLINK
#define	RTM_GETLINK		NL_RTM_GETLINK
#define	RTM_NEWADDR		NL_RTM_NEWADDR
#define	RTM_GETADDR		NL_RTM_GETADDR

struct ifaddrmsg {
	uint8_t		ifa_family;
	uint8_t		ifa_prefixlen;
	uint8_t		ifa_flags;
	uint8_t		ifa_scope;
	uint32_t	ifa_index;
};

#define	_NL_IFA_HDRLEN		((int)sizeof(struct ifaddrmsg))
#define	IFA_RTA(_ifa) \
	((struct rtattr *)NL_ITEM_DATA(_ifa, _NL_IFA_HDRLEN))
#define	IFA_PAYLOAD(_hdr)	NLMSG_PAYLOAD(_hdr, _NL_IFA_HDRLEN)

enum {
	IFA_UNSPEC,
	IFA_ADDRESS		= 1,
	IFA_LOCAL		= 2,
	IFA_LABEL		= 3,
	IFA_BROADCAST		= 4,
};

struct ifinfomsg {
	unsigned char	ifi_family;
	unsigned char	__ifi_pad;
	unsigned short	ifi_type;
	int		ifi_index;
	unsigned	ifi_flags;
	unsigned	ifi_change;
};

#define	_IFINFO_HDRLEN		((int)sizeof(struct ifinfomsg))
#define	IFLA_RTA(_ifi) \
	((struct rtattr *)NL_ITEM_DATA(_ifi, _IFINFO_HDRLEN))
#define	IFLA_PAYLOAD(_hdr)	NLMSG_PAYLOAD(_hdr, _IFINFO_HDRLEN)

enum {
	IFLA_UNSPEC	= 0,
	IFLA_ADDRESS	= 1,
#define	IFLA_ADDRESS IFLA_ADDRESS
	IFLA_BROADCAST	= 2,
#define	IFLA_BROADCAST IFLA_BROADCAST
	IFLA_IFNAME	= 3,
#define	IFLA_IFNAME IFLA_IFNAME
};

struct rtattr {
	unsigned short	rta_len;
	unsigned short	rta_type;
};

#define	NL_RTA_ALIGN_SIZE	NL_ITEM_ALIGN_SIZE
#define	NL_RTA_ALIGN		NL_ITEM_ALIGN
#define	NL_RTA_HDRLEN		((int)sizeof(struct rtattr))
#define	NL_RTA_DATA(_rta)	NL_ITEM_DATA(_rta, NL_RTA_HDRLEN)

/* The lengths are size_t, as in the bionic callers; FreeBSD has int. */
#define	RTA_ALIGNTO		NL_RTA_ALIGN_SIZE
#define	RTA_ALIGN(_len)		NL_RTA_ALIGN(_len)
#define	_RTA_LEN(_rta)		((size_t)(_rta)->rta_len)
#define	_RTA_ALIGNED_LEN(_rta)	RTA_ALIGN(_RTA_LEN(_rta))
#define	RTA_OK(_rta, _len)	NL_ITEM_OK(_rta, _len, NL_RTA_HDRLEN, _RTA_LEN)
#define	RTA_NEXT(_rta, _len)	NL_ITEM_ITER(_rta, _len, _RTA_ALIGNED_LEN)
#define	RTA_LENGTH(_len)	(NL_RTA_HDRLEN + (_len))
#define	RTA_SPACE(_len)		RTA_ALIGN(RTA_LENGTH(_len))
#define	RTA_DATA(_rta)		NL_RTA_DATA(_rta)
#define	RTA_PAYLOAD(_rta)	(_RTA_LEN(_rta) - NL_RTA_HDRLEN)

struct rtgenmsg {
	unsigned char	rtgen_family;
};
