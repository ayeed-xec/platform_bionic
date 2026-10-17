/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
