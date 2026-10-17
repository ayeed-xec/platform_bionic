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
 * Linux system calls that FreeBSD has no counterpart for yet.  They fail
 * the way they would on a Linux kernel built without them, so callers
 * link and see an error they already handle.
 *
 * brk, sbrk		FreeBSD/arm64 has no break(2); malloc uses mmap(2).
 *			Both fail with ENOMEM.
 * clone		Needs an emulation on rfork(2) and thr_new(2).  fork()
 *			does not come through here.
 * rt_tgsigqueueinfo	sigqueue(2) can only target a process, not a thread.
 * syncfs		Nothing syncs a single file system by descriptor.
 */

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>

int	brk(void *);
void	*sbrk(intptr_t);
int	clone(int (*)(void *), void *, int, void *, ...);
int	__rt_tgsigqueueinfo(pid_t, pid_t, int, void *);
int	syncfs(int);

int
brk(void *addr)
{
	(void)addr;
	errno = ENOMEM;
	return (-1);
}

void *
sbrk(intptr_t increment)
{
	(void)increment;
	errno = ENOMEM;
	return ((void *)-1);
}

int
clone(int (*fn)(void *), void *stack, int flags, void *arg, ...)
{
	(void)fn;
	(void)stack;
	(void)flags;
	(void)arg;
	errno = ENOSYS;
	return (-1);
}

int
__rt_tgsigqueueinfo(pid_t pid, pid_t tid, int sig, void *info)
{
	(void)pid;
	(void)tid;
	(void)sig;
	(void)info;
	errno = ENOSYS;
	return (-1);
}

int
syncfs(int fd)
{
	(void)fd;
	errno = ENOSYS;
	return (-1);
}
