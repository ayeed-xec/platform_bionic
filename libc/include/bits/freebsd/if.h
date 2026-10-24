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
 * Interface flags and request structures from FreeBSD's <net/if.h>.  The
 * kernel reads these, so the values and layouts are FreeBSD's.  The SIOC*
 * ioctls will come with <sys/sockio.h>.  abi_check.c compares these with
 * the FreeBSD headers.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/socket.h>
#include <sys/types.h>

#define	IF_NAMESIZE	16
#define	IFNAMSIZ	IF_NAMESIZE

#define	IFF_UP		0x1
#define	IFF_BROADCAST	0x2
#define	IFF_DEBUG	0x4
#define	IFF_LOOPBACK	0x8
#define	IFF_POINTOPOINT	0x10
#define	IFF_NEEDSEPOCH	0x20
#define	IFF_DRV_RUNNING	0x40
#define	IFF_NOARP	0x80
#define	IFF_PROMISC	0x100
#define	IFF_ALLMULTI	0x200
#define	IFF_DRV_OACTIVE	0x400
#define	IFF_SIMPLEX	0x800
#define	IFF_LINK0	0x1000
#define	IFF_LINK1	0x2000
#define	IFF_LINK2	0x4000
#define	IFF_ALTPHYS	IFF_LINK2
#define	IFF_MULTICAST	0x8000
#define	IFF_CANTCONFIG	0x10000
#define	IFF_PPROMISC	0x20000
#define	IFF_MONITOR	0x40000
#define	IFF_STATICARP	0x80000
#define	IFF_STICKYARP	0x100000
#define	IFF_DYING	0x200000
#define	IFF_RENAMING	0x400000
#define	IFF_PALLMULTI	0x800000
#define	IFF_NETLINK_1	0x1000000
#define	IFF_RUNNING	IFF_DRV_RUNNING
#define	IFF_OACTIVE	IFF_DRV_OACTIVE

struct ifreq_buffer {
	size_t	length;
	void	*buffer;
};

struct ifreq_nv_req {
	unsigned int	buf_length;
	unsigned int	length;
	void		*buffer;
};

struct ifreq {
	char	ifr_name[IFNAMSIZ];
	union {
		struct sockaddr		ifru_addr;
		struct sockaddr		ifru_dstaddr;
		struct sockaddr		ifru_broadaddr;
		struct ifreq_buffer	ifru_buffer;
		short			ifru_flags[2];
		short			ifru_index;
		int			ifru_jid;
		int			ifru_metric;
		int			ifru_mtu;
		int			ifru_phys;
		int			ifru_media;
		char			*ifru_data;
		int			ifru_cap[2];
		unsigned int		ifru_fib;
		unsigned char		ifru_vlan_pcp;
		struct ifreq_nv_req	ifru_nv;
	} ifr_ifru;
};
#define	ifr_addr	ifr_ifru.ifru_addr
#define	ifr_dstaddr	ifr_ifru.ifru_dstaddr
#define	ifr_broadaddr	ifr_ifru.ifru_broadaddr
#define	ifr_buffer	ifr_ifru.ifru_buffer
#define	ifr_flags	ifr_ifru.ifru_flags[0]
#define	ifr_flagshigh	ifr_ifru.ifru_flags[1]
#define	ifr_jid		ifr_ifru.ifru_jid
#define	ifr_metric	ifr_ifru.ifru_metric
#define	ifr_mtu		ifr_ifru.ifru_mtu
#define	ifr_phys	ifr_ifru.ifru_phys
#define	ifr_media	ifr_ifru.ifru_media
#define	ifr_data	ifr_ifru.ifru_data
#define	ifr_reqcap	ifr_ifru.ifru_cap[0]
#define	ifr_curcap	ifr_ifru.ifru_cap[1]
#define	ifr_index	ifr_ifru.ifru_index
#define	ifr_fib		ifr_ifru.ifru_fib
#define	ifr_vlan_pcp	ifr_ifru.ifru_vlan_pcp
#define	ifr_lan_pcp	ifr_ifru.ifru_vlan_pcp
#define	ifr_cap_nv	ifr_ifru.ifru_nv

struct ifconf {
	int	ifc_len;
	union {
		char		*ifcu_buf;
		struct ifreq	*ifcu_req;
	} ifc_ifcu;
};
#define	ifc_buf		ifc_ifcu.ifcu_buf
#define	ifc_req		ifc_ifcu.ifcu_req

struct ifaliasreq {
	char			ifra_name[IFNAMSIZ];
	struct sockaddr		ifra_addr;
	struct sockaddr		ifra_broadaddr;
	struct sockaddr		ifra_mask;
	int			ifra_vhid;
};

struct ifdrv {
	char			ifd_name[IFNAMSIZ];
	unsigned long		ifd_cmd;
	size_t			ifd_len;
	void			*ifd_data;
};
