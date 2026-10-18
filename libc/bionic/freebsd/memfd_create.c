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
 * memfd_create(2) for bionic-freebsd, on shm_open2(2) with SHM_ANON as in
 * FreeBSD's libc.  The object grows on write(2), as a Linux memfd does.
 * MFD_HUGETLB is not supported: it fails with EINVAL, as an unknown flag
 * does.
 */

#include <sys/types.h>
#include <sys/mman.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define	MEMFD_PREFIX			"memfd:"

/* FreeBSD's values from <sys/mman.h>. */
#define	__FREEBSD_SHM_ANON		((char *)1)
#define	__FREEBSD_SHM_ALLOW_SEALING	0x00000001
#define	__FREEBSD_SHM_GROW_ON_WRITE	0x00000002

int	__shm_open2(const char *, int, mode_t, int, const char *);

int
memfd_create(const char *name, unsigned int flags)
{
	char shm_name[NAME_MAX + 1];
	int oflags, shmflags;

	if (name == NULL) {
		errno = EFAULT;
		return (-1);
	}
	if (strlen(name) > NAME_MAX - (sizeof(MEMFD_PREFIX) - 1) ||
	    (flags & ~(MFD_CLOEXEC | MFD_ALLOW_SEALING)) != 0) {
		errno = EINVAL;
		return (-1);
	}
	snprintf(shm_name, sizeof(shm_name), "%s%s", MEMFD_PREFIX, name);
	oflags = O_RDWR;
	if ((flags & MFD_CLOEXEC) != 0)
		oflags |= O_CLOEXEC;
	shmflags = __FREEBSD_SHM_GROW_ON_WRITE;
	if ((flags & MFD_ALLOW_SEALING) != 0)
		shmflags |= __FREEBSD_SHM_ALLOW_SEALING;
	return (__shm_open2(__FREEBSD_SHM_ANON, oflags, 0, shmflags, shm_name));
}
