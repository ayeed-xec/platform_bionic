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
 * The credential interface of FreeBSD unix(4) sockets, from FreeBSD
 * sys/sys/un.h, sys/sys/socket.h and sys/sys/ucred.h, under private names.
 * setsockopt(), getsockopt() and recvmsg() map the Linux names of
 * <sys/socket.h> to it.  The options are at level SOL_LOCAL, which is 0:
 * on a socket of another domain, level 0 is that protocol's level, as it
 * is for FreeBSD's own Linux ABI.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

#define	__FREEBSD_SOL_LOCAL			0
#define	__FREEBSD_LOCAL_PEERCRED		1
#define	__FREEBSD_LOCAL_CREDS_PERSISTENT	3

/* The control message of LOCAL_CREDS_PERSISTENT, at level SOL_SOCKET. */
#define	__FREEBSD_SCM_CREDS2		0x08
#define	__FREEBSD_CMGROUP_MAX		16

struct __freebsd_sockcred2 {
	int	sc_version;
	pid_t	sc_pid;
	uid_t	sc_uid;		/* real */
	uid_t	sc_euid;
	gid_t	sc_gid;		/* real */
	gid_t	sc_egid;
	int	sc_ngroups;
	gid_t	sc_groups[1];
};

#define	__FREEBSD_SOCKCRED2SIZE(ngrps) \
	(sizeof(struct __freebsd_sockcred2) + sizeof(gid_t) * ((ngrps) - 1))

/* The result of LOCAL_PEERCRED: cr_groups[0] is the effective gid. */
#define	__FREEBSD_XU_NGROUPS		16
#define	__FREEBSD_XUCRED_VERSION	0

struct __freebsd_xucred {
	u_int	cr_version;
	uid_t	cr_uid;		/* effective */
	short	cr_ngroups;	/* with the effective gid */
	gid_t	cr_groups[__FREEBSD_XU_NGROUPS];
	union {
		void	*_cr_unused1;
		pid_t	cr_pid;
	};
};

_Static_assert(sizeof(struct __freebsd_sockcred2) == 32,
    "struct sockcred2 of FreeBSD");
_Static_assert(sizeof(struct __freebsd_xucred) == 88,
    "struct xucred of FreeBSD");
