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
 * getpriority(2) and setpriority(2) for bionic-freebsd.
 *
 * Linux keeps a nice value for each thread, and PRIO_PROCESS names one
 * thread: 0 is the calling thread, and an id is a thread id, so a process
 * id names the main thread.  FreeBSD's PRIO_THREAD has the same rule, so
 * PRIO_PROCESS becomes PRIO_THREAD.  PRIO_PGRP and PRIO_USER act on whole
 * processes on both systems and pass unchanged.  FreeBSD returns the nice
 * value itself, as libc does on Linux.
 */

#include <sys/types.h>
#include <sys/resource.h>

/* FreeBSD <sys/resource.h>. */
#define	FREEBSD_PRIO_THREAD	3

int	__getpriority(int, id_t);
int	__setpriority(int, id_t, int);

static int
freebsd_which(int which)
{
	return (which == PRIO_PROCESS ? FREEBSD_PRIO_THREAD : which);
}

int
getpriority(int which, id_t who)
{
	return (__getpriority(freebsd_which(which), who));
}

int
setpriority(int which, id_t who, int prio)
{
	return (__setpriority(freebsd_which(which), who, prio));
}
