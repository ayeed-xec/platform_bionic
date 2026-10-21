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
 * Netlink for <linux/netlink.h>, from FreeBSD <netlink/netlink.h>: FreeBSD
 * netlink(4) uses the Linux message format.  struct sockaddr_nl has the
 * FreeBSD length byte.
 */

#pragma once

#include <stdint.h>
#include <sys/socket.h>

struct sockaddr_nl {
	uint8_t		nl_len;
	sa_family_t	nl_family;
	uint16_t	nl_pad;
	uint32_t	nl_pid;
	uint32_t	nl_groups;
};

#define	SOL_NETLINK		270

/* The netlink families.  FreeBSD has NETLINK_ROUTE and NETLINK_GENERIC. */
#define	NETLINK_ROUTE		0
#define	NETLINK_AUDIT		9
#define	NETLINK_KOBJECT_UEVENT	15
#define	NETLINK_GENERIC		16

struct nlmsghdr {
	uint32_t	nlmsg_len;
	uint16_t	nlmsg_type;
	uint16_t	nlmsg_flags;
	uint32_t	nlmsg_seq;
	uint32_t	nlmsg_pid;
};

/* Flags of nlmsg_flags. */
#define	NLM_F_REQUEST		0x01
#define	NLM_F_MULTI		0x02
#define	NLM_F_ACK		0x04
#define	NLM_F_ECHO		0x08
#define	NLM_F_ROOT		0x100
#define	NLM_F_MATCH		0x200
#define	NLM_F_DUMP		(NLM_F_ROOT | NLM_F_MATCH)

/* Types of nlmsg_type. */
#define	NLMSG_NOOP		0x1
#define	NLMSG_ERROR		0x2
#define	NLMSG_DONE		0x3
#define	NLMSG_OVERRUN		0x4
#define	NLMSG_MIN_TYPE		0x10

struct nlmsgerr {
	int		error;
	struct nlmsghdr	msg;
};

/*
 * Item helpers from FreeBSD's <netlink/netlink.h>.  A message and an
 * attribute are both items: a header that holds the length, then the data,
 * on four-byte boundaries.  That header carries the license above and these
 * notices:
 *
 * Copyright (c) 2021 Ng Peng Nam Sean
 * Copyright (c) 2022 Alexander V. Chernikov <melifaro@FreeBSD.org>
 */
#define	NL_ITEM_ALIGN_SIZE	sizeof(uint32_t)
#define	NL_ITEM_ALIGN(_len)	__builtin_align_up(_len, NL_ITEM_ALIGN_SIZE)
#define	NL_ITEM_DATA(_ptr, _off) \
	((void *)((char *)(_ptr) + (_off)))
#define	NL_ITEM_OK(_ptr, _len, _hlen, _LEN_M) \
	((_len) >= (_hlen) && _LEN_M(_ptr) >= (_hlen) && _LEN_M(_ptr) <= (_len))
#define	NL_ITEM_NEXT(_ptr, _LEN_M) \
	((__typeof(_ptr))((char *)(_ptr) + _LEN_M(_ptr)))
#define	NL_ITEM_ITER(_ptr, _len, _LEN_MACRO) \
	((_len) -= _LEN_MACRO(_ptr), NL_ITEM_NEXT(_ptr, _LEN_MACRO))

/* The macros of FreeBSD: the lengths are unsigned. */
#define	NLMSG_ALIGNTO		sizeof(uint32_t)
#define	NLMSG_ALIGN(len) \
	(((len) + NLMSG_ALIGNTO - 1) & ~(NLMSG_ALIGNTO - 1))
#define	NLMSG_HDRLEN		(sizeof(struct nlmsghdr))
#define	NLMSG_LENGTH(len)	((len) + NLMSG_HDRLEN)
#define	NLMSG_SPACE(len)	NLMSG_ALIGN(NLMSG_LENGTH(len))
#define	NLMSG_DATA(nlh)		((void *)((char *)(nlh) + NLMSG_HDRLEN))
#define	NLMSG_NEXT(nlh, len) \
	((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
	    (struct nlmsghdr *)((char *)(nlh) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define	NLMSG_OK(nlh, len) \
	((len) >= NLMSG_HDRLEN && (nlh)->nlmsg_len >= NLMSG_HDRLEN && \
	    (nlh)->nlmsg_len <= (len))
#define	NLMSG_PAYLOAD(nlh, len)	((nlh)->nlmsg_len - NLMSG_SPACE(len))
