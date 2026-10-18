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
 * __specialfd(2) of FreeBSD makes eventfd, inotify and epoll descriptors.
 * The types and the argument structures are the ones of the FreeBSD kernel
 * (<sys/specialfd.h>).
 */

#pragma once

#include <stddef.h>
#include <sys/cdefs.h>

#define	SPECIALFD_EVENTFD	1
#define	SPECIALFD_INOTIFY	2
#define	SPECIALFD_EPOLL		3

struct specialfd_eventfd {
	unsigned int	initval;
	int		flags;
};

struct specialfd_inotify {
	int	flags;
};

struct specialfd_epoll {
	int	flags;
};

__BEGIN_DECLS
int	__specialfd(int __type, const void* __req, size_t __len);
__END_DECLS
