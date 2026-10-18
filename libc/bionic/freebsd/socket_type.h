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
 * The socket type that bionic-freebsd gives to socket(2) and socketpair(2).
 * A FreeBSD AF_UNIX SOCK_SEQPACKET socket ends a record only at MSG_EOR.
 * Android code needs the Linux rules: each send is one record, and a short
 * receive drops the rest of the record.  The FreeBSD creation flag
 * SOCK_ATOMIC gives them.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/socket.h>

#define	__FREEBSD_SOCK_ATOMIC	0x08000000	/* SOCK_ATOMIC of FreeBSD */

static inline int
__freebsd_socket_type(int domain, int type)
{
	if (domain == AF_UNIX && (type & ~(SOCK_CLOEXEC | SOCK_NONBLOCK |
	    SOCK_CLOFORK)) == SOCK_SEQPACKET)
		return (type | __FREEBSD_SOCK_ATOMIC);
	return (type);
}
