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
 * klogctl		The kern.msgbuf sysctl reads the whole kernel message
 *			buffer, but FreeBSD has no read that consumes it and no
 *			count of unread bytes.  ENOSYS.
 * mseal		Nothing stops later mprotect(2) or munmap(2) calls on a
 *			range.  ENOSYS, which callers take as an older kernel.
 * unshare, setns	FreeBSD has no namespaces.  EINVAL, as on a kernel
 *			built without them; unshare() with no flag does nothing.
 *			unshare() also gives EINVAL for CLONE_FS, CLONE_FILES
 *			and CLONE_SYSVSEM, which each Linux kernel takes.
 */

#define	_GNU_SOURCE	/* unshare() and setns() of <sched.h> */

#include <errno.h>
#include <sched.h>
#include <stdint.h>
#include <sys/types.h>

int	brk(void *);
void	*sbrk(intptr_t);
int	mseal(void *, size_t, unsigned long);
int	klogctl(int, char *, int);

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
mseal(void *addr, size_t len, unsigned long flags)
{
	(void)addr;
	(void)len;
	(void)flags;
	errno = ENOSYS;
	return (-1);
}

int
klogctl(int type, char *buf, int len)
{
	(void)type;
	(void)buf;
	(void)len;
	errno = ENOSYS;
	return (-1);
}

int
unshare(int flags)
{
	if (flags == 0)
		return (0);
	errno = EINVAL;
	return (-1);
}

int
setns(int fd, int nstype)
{
	(void)fd;
	(void)nstype;
	errno = EINVAL;
	return (-1);
}
