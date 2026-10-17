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
 * madvise(2) for bionic-freebsd.  The MADV_* values keep the Android
 * numbers, and libc translates them to FreeBSD (ADR 0002, decision 1.3).
 * The fork advice goes to minherit(2).  Advice that FreeBSD does not have
 * fails with EINVAL, as on a Linux kernel without the feature.
 *
 * FreeBSD MADV_DONTNEED does not zero anonymous pages (probe P3).  Code
 * that needs zeroed pages must not use it until the emulation does that.
 */

#include <errno.h>
#include <stddef.h>
#include <sys/mman.h>

int	__madvise(void *, size_t, int);
int	minherit(void *, size_t, int);

int
madvise(void *addr, size_t len, int advice)
{
	switch (advice) {
	case MADV_NORMAL:
		return (__madvise(addr, len, __FREEBSD_MADV_NORMAL));
	case MADV_RANDOM:
		return (__madvise(addr, len, __FREEBSD_MADV_RANDOM));
	case MADV_SEQUENTIAL:
		return (__madvise(addr, len, __FREEBSD_MADV_SEQUENTIAL));
	case MADV_WILLNEED:
		return (__madvise(addr, len, __FREEBSD_MADV_WILLNEED));
	case MADV_DONTNEED:
		return (__madvise(addr, len, __FREEBSD_MADV_DONTNEED));
	case MADV_FREE:
		return (__madvise(addr, len, __FREEBSD_MADV_FREE));
	case MADV_DONTDUMP:
		return (__madvise(addr, len, __FREEBSD_MADV_NOCORE));
	case MADV_DODUMP:
		return (__madvise(addr, len, __FREEBSD_MADV_CORE));
	case MADV_DONTFORK:
		return (minherit(addr, len, INHERIT_NONE));
	case MADV_DOFORK:
	case MADV_KEEPONFORK:
		return (minherit(addr, len, INHERIT_COPY));
	case MADV_WIPEONFORK:
		return (minherit(addr, len, INHERIT_ZERO));
	}
	errno = EINVAL;
	return (-1);
}
