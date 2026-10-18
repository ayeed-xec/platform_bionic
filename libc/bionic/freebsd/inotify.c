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
 * inotify for bionic-freebsd.  FreeBSD has a native inotify with the events
 * and flags of Linux: inotify_init1() makes the descriptor with
 * __specialfd(2), and inotify_add_watch() is inotify_add_watch_at(2) with
 * AT_FDCWD.  inotify_rm_watch() is a system call stub.
 */

#include <sys/inotify.h>

#include <fcntl.h>
#include <stdint.h>

#include "specialfd.h"

int	__inotify_add_watch_at(int, int, const char *, uint32_t);

int
inotify_init1(int flags)
{
	struct specialfd_inotify args;

	args.flags = flags;
	return (__specialfd(SPECIALFD_INOTIFY, &args, sizeof(args)));
}

int
inotify_add_watch(int fd, const char *path, uint32_t mask)
{
	return (__inotify_add_watch_at(fd, AT_FDCWD, path, mask));
}
