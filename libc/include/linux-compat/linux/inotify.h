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
 * The inotify interface of Linux, for <sys/inotify.h>.  The FreeBSD kernel
 * has the same events, flags and struct inotify_event (FreeBSD
 * <sys/inotify.h>); IN_CLOEXEC and IN_NONBLOCK are the FreeBSD O_CLOEXEC
 * and O_NONBLOCK.
 */

#pragma once

#include <stdint.h>

#define	IN_NONBLOCK		0x00000004
#define	IN_CLOEXEC		0x00100000

struct inotify_event {
	int		wd;
	uint32_t	mask;
	uint32_t	cookie;
	uint32_t	len;
	char		name[];
};

/* Events. */
#define	IN_ACCESS		0x00000001
#define	IN_MODIFY		0x00000002
#define	IN_ATTRIB		0x00000004
#define	IN_CLOSE_WRITE		0x00000008
#define	IN_CLOSE_NOWRITE	0x00000010
#define	IN_CLOSE		(IN_CLOSE_WRITE | IN_CLOSE_NOWRITE)
#define	IN_OPEN			0x00000020
#define	IN_MOVED_FROM		0x00000040
#define	IN_MOVED_TO		0x00000080
#define	IN_MOVE			(IN_MOVED_FROM | IN_MOVED_TO)
#define	IN_CREATE		0x00000100
#define	IN_DELETE		0x00000200
#define	IN_DELETE_SELF		0x00000400
#define	IN_MOVE_SELF		0x00000800
#define	IN_ALL_EVENTS		0x00000fff

/* Results that the kernel sets. */
#define	IN_UNMOUNT		0x00002000
#define	IN_Q_OVERFLOW		0x00004000
#define	IN_IGNORED		0x00008000
#define	IN_ISDIR		0x40000000

/* Flags of inotify_add_watch(). */
#define	IN_ONLYDIR		0x01000000
#define	IN_DONT_FOLLOW		0x02000000
#define	IN_EXCL_UNLINK		0x04000000
#define	IN_MASK_CREATE		0x10000000
#define	IN_MASK_ADD		0x20000000
#define	IN_ONESHOT		0x80000000
