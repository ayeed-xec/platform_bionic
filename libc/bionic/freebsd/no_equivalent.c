/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Linux system calls with no FreeBSD equivalent yet (kind G in
 * SYSCALLS.FREEBSD.TXT, docs/specs/syscall-parity.md).  The bionic code that
 * calls them links, and the call fails as on a Linux kernel without the
 * system call.
 *
 * brk	FreeBSD arm64 has no break(2): the heap uses mmap(2).  brk() and
 *	sbrk() fail with ENOMEM.
 * clone	The emulation on rfork(2) and thr_new(2) comes later.  fork()
 *	does not use it.
 * rt_tgsigqueueinfo	sigqueue(2) sends to a process, not to a thread.
 * syncfs	FreeBSD has no call that syncs one file system by file
 *	descriptor.  syncfs() fails with ENOSYS.
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
