/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Socket constants and structures of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/socket.h.  The CMSG_* alignment is the _ALIGNBYTES of FreeBSD
 * sys/arm64/include/_align.h.  The FreeBSD kernel reads these values, so
 * they are the FreeBSD values.  Names that only Linux has are not defined.
 * struct sockaddr_storage is in bits/sockaddr_storage.h.  abi_check.c
 * checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>
#include <bits/sa_family_t.h>
#include <bits/freebsd/uio.h>

/* Socket types and the flags that are or-ed into them. */
#define	SOCK_STREAM		1
#define	SOCK_DGRAM		2
#define	SOCK_RAW		3
#define	SOCK_RDM		4
#define	SOCK_SEQPACKET		5
#define	SOCK_CLOEXEC		0x10000000
#define	SOCK_NONBLOCK		0x20000000
#define	SOCK_CLOFORK		0x40000000

/* Option level and option names for setsockopt(2). */
#define	SOL_SOCKET		0xffff

#define	SO_DEBUG		0x00000001
#define	SO_ACCEPTCONN		0x00000002
#define	SO_REUSEADDR		0x00000004
#define	SO_KEEPALIVE		0x00000008
#define	SO_DONTROUTE		0x00000010
#define	SO_BROADCAST		0x00000020
#define	SO_USELOOPBACK		0x00000040
#define	SO_LINGER		0x00000080
#define	SO_OOBINLINE		0x00000100
#define	SO_REUSEPORT		0x00000200
#define	SO_TIMESTAMP		0x00000400
#define	SO_NOSIGPIPE		0x00000800
#define	SO_ACCEPTFILTER		0x00001000
#define	SO_BINTIME		0x00002000
#define	SO_NO_OFFLOAD		0x00004000
#define	SO_NO_DDP		0x00008000
#define	SO_REUSEPORT_LB		0x00010000
#define	SO_RERROR		0x00020000

#define	SO_SNDBUF		0x1001
#define	SO_RCVBUF		0x1002
#define	SO_SNDLOWAT		0x1003
#define	SO_RCVLOWAT		0x1004
#define	SO_SNDTIMEO		0x1005
#define	SO_RCVTIMEO		0x1006
#define	SO_ERROR		0x1007
#define	SO_TYPE			0x1008
#define	SO_LABEL		0x1009
#define	SO_PEERLABEL		0x1010
#define	SO_LISTENQLIMIT		0x1011
#define	SO_LISTENQLEN		0x1012
#define	SO_LISTENINCQLEN	0x1013
#define	SO_FIB			0x1014
#define	SO_SETFIB		SO_FIB
#define	SO_USER_COOKIE		0x1015
#define	SO_PROTOCOL		0x1016
#define	SO_PROTOTYPE		SO_PROTOCOL
#define	SO_TS_CLOCK		0x1017
#define	SO_MAX_PACING_RATE	0x1018
#define	SO_DOMAIN		0x1019
#define	SO_SPLICE		0x1023

/* Values of the SO_TS_CLOCK option. */
#define	SO_TS_REALTIME_MICRO	0
#define	SO_TS_BINTIME		1
#define	SO_TS_REALTIME		2
#define	SO_TS_MONOTONIC		3
#define	SO_TS_DEFAULT		SO_TS_REALTIME_MICRO
#define	SO_TS_CLOCK_MAX		SO_TS_MONOTONIC

/* Address families. */
#define	AF_UNSPEC		0
#define	AF_UNIX			1
#define	AF_LOCAL		AF_UNIX
#define	AF_INET			2
#define	AF_IMPLINK		3
#define	AF_PUP			4
#define	AF_CHAOS		5
#define	AF_NETBIOS		6
#define	AF_ISO			7
#define	AF_OSI			AF_ISO
#define	AF_ECMA			8
#define	AF_DATAKIT		9
#define	AF_CCITT		10
#define	AF_SNA			11
#define	AF_DECnet		12
#define	AF_DLI			13
#define	AF_LAT			14
#define	AF_HYLINK		15
#define	AF_APPLETALK		16
#define	AF_ROUTE		17
#define	AF_LINK			18
#define	pseudo_AF_XTP		19
#define	AF_COIP			20
#define	AF_CNT			21
#define	pseudo_AF_RTIP		22
#define	AF_IPX			23
#define	AF_SIP			24
#define	pseudo_AF_PIP		25
#define	AF_ISDN			26
#define	AF_E164			AF_ISDN
#define	pseudo_AF_KEY		27
#define	AF_INET6		28
#define	AF_NATM			29
#define	AF_ATM			30
#define	pseudo_AF_HDRCMPLT	31
#define	AF_NETGRAPH		32
#define	AF_SLOW			33
#define	AF_SCLUSTER		34
#define	AF_ARP			35
#define	AF_BLUETOOTH		36
#define	AF_IEEE80211		37
#define	AF_NETLINK		38
#define	AF_INET_SDP		40
#define	AF_INET6_SDP		42
#define	AF_HYPERV		43
#define	AF_DIVERT		44
#define	AF_IPFWLOG		46
#define	AF_MAX			46

/* Protocol families. */
#define	PF_UNSPEC		AF_UNSPEC
#define	PF_LOCAL		AF_LOCAL
#define	PF_UNIX			PF_LOCAL
#define	PF_INET			AF_INET
#define	PF_IMPLINK		AF_IMPLINK
#define	PF_PUP			AF_PUP
#define	PF_CHAOS		AF_CHAOS
#define	PF_NETBIOS		AF_NETBIOS
#define	PF_ISO			AF_ISO
#define	PF_OSI			AF_ISO
#define	PF_ECMA			AF_ECMA
#define	PF_DATAKIT		AF_DATAKIT
#define	PF_CCITT		AF_CCITT
#define	PF_SNA			AF_SNA
#define	PF_DECnet		AF_DECnet
#define	PF_DLI			AF_DLI
#define	PF_LAT			AF_LAT
#define	PF_HYLINK		AF_HYLINK
#define	PF_APPLETALK		AF_APPLETALK
#define	PF_ROUTE		AF_ROUTE
#define	PF_LINK			AF_LINK
#define	PF_XTP			pseudo_AF_XTP
#define	PF_COIP			AF_COIP
#define	PF_CNT			AF_CNT
#define	PF_SIP			AF_SIP
#define	PF_IPX			AF_IPX
#define	PF_RTIP			pseudo_AF_RTIP
#define	PF_PIP			pseudo_AF_PIP
#define	PF_ISDN			AF_ISDN
#define	PF_KEY			pseudo_AF_KEY
#define	PF_INET6		AF_INET6
#define	PF_NATM			AF_NATM
#define	PF_ATM			AF_ATM
#define	PF_NETGRAPH		AF_NETGRAPH
#define	PF_SLOW			AF_SLOW
#define	PF_SCLUSTER		AF_SCLUSTER
#define	PF_ARP			AF_ARP
#define	PF_BLUETOOTH		AF_BLUETOOTH
#define	PF_IEEE80211		AF_IEEE80211
#define	PF_NETLINK		AF_NETLINK
#define	PF_INET_SDP		AF_INET_SDP
#define	PF_INET6_SDP		AF_INET6_SDP
#define	PF_DIVERT		AF_DIVERT
#define	PF_IPFWLOG		AF_IPFWLOG
#define	PF_MAX			AF_MAX

/* Longest queue for listen(2). */
#define	SOMAXCONN		128

/* Flags of the msg_flags field and of the send and receive calls. */
#define	MSG_OOB			0x00000001
#define	MSG_PEEK		0x00000002
#define	MSG_DONTROUTE		0x00000004
#define	MSG_EOR			0x00000008
#define	MSG_TRUNC		0x00000010
#define	MSG_CTRUNC		0x00000020
#define	MSG_WAITALL		0x00000040
#define	MSG_DONTWAIT		0x00000080
#define	MSG_EOF			0x00000100
#define	MSG_NOTIFICATION	0x00002000
#define	MSG_NOSIGNAL		0x00020000
#define	MSG_CMSG_CLOEXEC	0x00040000
#define	MSG_WAITFORONE		0x00080000
#define	MSG_CMSG_CLOFORK	0x00400000

/* Types of the control messages at the SOL_SOCKET level. */
#define	SCM_RIGHTS		0x01
#define	SCM_TIMESTAMP		0x02
#define	SCM_REALTIME		0x05
#define	SCM_MONOTONIC		0x06

/* The how argument of shutdown(2). */
enum shutdown_how {
	SHUT_RD = 0,
#define	SHUT_RD		SHUT_RD
	SHUT_WR,
#define	SHUT_WR		SHUT_WR
	SHUT_RDWR
#define	SHUT_RDWR	SHUT_RDWR
};

struct sockaddr {
	unsigned char	sa_len;
	sa_family_t	sa_family;
	char		sa_data[14];
};

struct linger {
	int	l_onoff;
	int	l_linger;
};

struct msghdr {
	void		*msg_name;
	socklen_t	msg_namelen;
	struct iovec	*msg_iov;
	int		msg_iovlen;
	void		*msg_control;
	socklen_t	msg_controllen;
	int		msg_flags;
};

struct cmsghdr {
	socklen_t	cmsg_len;
	int		cmsg_level;
	int		cmsg_type;
};

/*
 * The control message macros.  A control message and its data start at
 * a multiple of sizeof(long long) bytes.
 */
#define	__CMSG_ALIGNBYTES	(sizeof(long long) - 1)
#define	CMSG_ALIGN(n) \
	(((size_t)(n) + __CMSG_ALIGNBYTES) & ~__CMSG_ALIGNBYTES)

#define	CMSG_DATA(cmsg) \
	((unsigned char *)(cmsg) + CMSG_ALIGN(sizeof(struct cmsghdr)))

#define	CMSG_FIRSTHDR(mhdr) \
	((mhdr)->msg_controllen >= sizeof(struct cmsghdr) ? \
	    (struct cmsghdr *)(mhdr)->msg_control : (struct cmsghdr *)0)

#define	CMSG_NXTHDR(mhdr, cmsg) \
	((char *)(cmsg) == (char *)0 ? CMSG_FIRSTHDR(mhdr) : \
	    ((char *)(cmsg) + \
	    CMSG_ALIGN(((struct cmsghdr *)(cmsg))->cmsg_len) + \
	    CMSG_ALIGN(sizeof(struct cmsghdr)) > \
	    (char *)(mhdr)->msg_control + (mhdr)->msg_controllen) ? \
	    (struct cmsghdr *)0 : \
	    (struct cmsghdr *)(void *)((char *)(cmsg) + \
	    CMSG_ALIGN(((struct cmsghdr *)(cmsg))->cmsg_len)))

#define	CMSG_SPACE(len) \
	(CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#define	CMSG_LEN(len) \
	(CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))
