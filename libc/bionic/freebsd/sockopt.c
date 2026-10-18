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
 * setsockopt(2) and getsockopt(2) for bionic-freebsd.  The Linux
 * credential options of <sys/socket.h> each become one FreeBSD unix(4)
 * option (local_creds.h):
 *
 * SO_PASSCRED		LOCAL_CREDS_PERSISTENT: recvmsg() then gives
 *			SCM_CREDENTIALS with each receive.
 * SO_PEERCRED		LOCAL_PEERCRED, as a struct ucred: the pid, the
 *			effective uid and the effective gid of the peer.
 * SO_PEERGROUPS	LOCAL_PEERCRED, as the supplementary groups of the
 *			peer.  FreeBSD keeps at most 15 of them.
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
#include <string.h>

#include "local_creds.h"

int	__sys_getsockopt(int, int, int, void *, socklen_t *);
int	__sys_setsockopt(int, int, int, const void *, socklen_t);

static int
peer_xucred(int fd, struct __freebsd_xucred *xu)
{
	socklen_t len;

	len = sizeof(*xu);
	if (__sys_getsockopt(fd, __FREEBSD_SOL_LOCAL,
	    __FREEBSD_LOCAL_PEERCRED, xu, &len) == -1)
		return (-1);
	/* Level 0 of a socket of another domain gives other data. */
	if (len != sizeof(*xu) || xu->cr_version != __FREEBSD_XUCRED_VERSION ||
	    xu->cr_ngroups < 1 || xu->cr_ngroups > __FREEBSD_XU_NGROUPS) {
		errno = ENOPROTOOPT;
		return (-1);
	}
	return (0);
}

static int
peercred(int fd, void *val, socklen_t *lenp)
{
	struct __freebsd_xucred xu;
	struct ucred uc;

	if (peer_xucred(fd, &xu) != 0)
		return (-1);
	uc.pid = xu.cr_pid;
	uc.uid = xu.cr_uid;
	uc.gid = xu.cr_groups[0];
	if (*lenp > sizeof(uc))
		*lenp = sizeof(uc);
	memcpy(val, &uc, *lenp);
	return (0);
}

static int
peergroups(int fd, void *val, socklen_t *lenp)
{
	struct __freebsd_xucred xu;
	socklen_t len;

	if (peer_xucred(fd, &xu) != 0)
		return (-1);
	len = (xu.cr_ngroups - 1) * sizeof(gid_t);
	if (*lenp < len) {
		*lenp = len;
		errno = ERANGE;
		return (-1);
	}
	memcpy(val, &xu.cr_groups[1], len);
	*lenp = len;
	return (0);
}

int
getsockopt(int fd, int level, int name, void *val, socklen_t *lenp)
{
	if (level == SOL_SOCKET) {
		switch (name) {
		case SO_PASSCRED:
			return (__sys_getsockopt(fd, __FREEBSD_SOL_LOCAL,
			    __FREEBSD_LOCAL_CREDS_PERSISTENT, val, lenp));
		case SO_PEERCRED:
			return (peercred(fd, val, lenp));
		case SO_PEERGROUPS:
			return (peergroups(fd, val, lenp));
		}
	}
	return (__sys_getsockopt(fd, level, name, val, lenp));
}

int
setsockopt(int fd, int level, int name, const void *val, socklen_t len)
{
	if (level == SOL_SOCKET && name == SO_PASSCRED)
		return (__sys_setsockopt(fd, __FREEBSD_SOL_LOCAL,
		    __FREEBSD_LOCAL_CREDS_PERSISTENT, val, len));
	return (__sys_setsockopt(fd, level, name, val, len));
}
