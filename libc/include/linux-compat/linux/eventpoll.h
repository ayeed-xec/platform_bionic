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
 * The epoll constants of Linux, for <sys/epoll.h>.  The FreeBSD kernel
 * takes the same values (<sys/epoll.h> of FreeBSD), except EPOLL_CLOEXEC,
 * which is O_CLOEXEC.  struct epoll_event is in <bits/epoll_event.h>.
 */

#pragma once

#include <bits/epoll_event.h>
#include <fcntl.h>

#define	EPOLL_CLOEXEC	O_CLOEXEC

#define	EPOLL_CTL_ADD	1
#define	EPOLL_CTL_DEL	2
#define	EPOLL_CTL_MOD	3

#define	EPOLLIN		0x00000001U
#define	EPOLLPRI	0x00000002U
#define	EPOLLOUT	0x00000004U
#define	EPOLLERR	0x00000008U
#define	EPOLLHUP	0x00000010U
#define	EPOLLNVAL	0x00000020U
#define	EPOLLRDNORM	0x00000040U
#define	EPOLLRDBAND	0x00000080U
#define	EPOLLWRNORM	0x00000100U
#define	EPOLLWRBAND	0x00000200U
#define	EPOLLMSG	0x00000400U
#define	EPOLLRDHUP	0x00002000U
#define	EPOLLEXCLUSIVE	(1U << 28)
#define	EPOLLWAKEUP	(1U << 29)
#define	EPOLLONESHOT	(1U << 30)
#define	EPOLLET		(1U << 31)
