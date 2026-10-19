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
 * mmap(2) values.  The kernel reads PROT_*, MAP_*, MS_*, MCL_*, MFD_* and
 * the minherit(2) INHERIT_* values, so they have the values of FreeBSD
 * sys/sys/mman.h; abi_check.c checks them.  libc translates MADV_* and
 * MLOCK_* (ADR 0002 decision 1.3), so they keep the Android values.  The
 * kernel reads MREMAP_*, which have the FreeBSD values of mremap(2); they
 * are also the Android values.
 */

#pragma once

#include <sys/cdefs.h>

#define	PROT_NONE		0x00
#define	PROT_READ		0x01
#define	PROT_WRITE		0x02
#define	PROT_EXEC		0x04
#define	PROT_MAX(prot)		((prot) << 16)
/*
 * FreeBSD has no MTE mappings and no PROT_GROWSDOWN.  These keep the Android
 * values: mmap(2) and mprotect(2) reject them with EINVAL.  bionic uses them
 * only for MTE, and the kernel never sets HWCAP2_MTE.
 */
#define	PROT_MTE		0x20
#define	PROT_GROWSDOWN		0x01000000
/*
 * PROT_BTI keeps the Android value too.  mmap() and mprotect() give it to
 * the kernel as the FreeBSD PROT_BTI, 0x40 (FreeBSD reserves 0x10).
 */
#define	PROT_BTI		0x10

#define	MAP_SHARED		0x0001
#define	MAP_PRIVATE		0x0002
#define	MAP_FIXED		0x0010
#define	MAP_STACK		0x0400
#define	MAP_NOSYNC		0x0800
#define	MAP_FILE		0x0000
#define	MAP_ANONYMOUS		0x1000
#define	MAP_GUARD		0x00002000
#define	MAP_EXCL		0x00004000
#define	MAP_NOCORE		0x00020000
#define	MAP_PREFAULT_READ	0x00040000
#define	MAP_ALIGNED(n)		((n) << 24)
#define	MAP_ALIGNED_SUPER	MAP_ALIGNED(1)
/* Linux names. */
#define	MAP_FIXED_NOREPLACE	(MAP_FIXED | MAP_EXCL)
#define	MAP_NORESERVE		0	/* FreeBSD reserves no swap. */
#define	MAP_POPULATE		0	/* A hint only. */

/* The FreeBSD madvise(2) values, for the translation in libc. */
#define	__FREEBSD_MADV_NORMAL		0
#define	__FREEBSD_MADV_RANDOM		1
#define	__FREEBSD_MADV_SEQUENTIAL	2
#define	__FREEBSD_MADV_WILLNEED		3
#define	__FREEBSD_MADV_DONTNEED		4
#define	__FREEBSD_MADV_FREE		5
#define	__FREEBSD_MADV_NOCORE		8
#define	__FREEBSD_MADV_CORE		9

#define	INHERIT_SHARE		0
#define	INHERIT_COPY		1
#define	INHERIT_NONE		2
#define	INHERIT_ZERO		3

#define	MS_SYNC			0x0000
#define	MS_ASYNC		0x0001
#define	MS_INVALIDATE		0x0002

#define	MCL_CURRENT		0x0001
#define	MCL_FUTURE		0x0002

#define	MFD_CLOEXEC		0x00000001
#define	MFD_ALLOW_SEALING	0x00000002
#define	MFD_HUGETLB		0x00000004

/* Android values; the madvise() wrapper translates them. */
#define	MADV_NORMAL		0
#define	MADV_RANDOM		1
#define	MADV_SEQUENTIAL		2
#define	MADV_WILLNEED		3
#define	MADV_DONTNEED		4
#define	MADV_FREE		8
#define	MADV_REMOVE		9
#define	MADV_DONTFORK		10
#define	MADV_DOFORK		11
#define	MADV_MERGEABLE		12
#define	MADV_UNMERGEABLE	13
#define	MADV_HUGEPAGE		14
#define	MADV_NOHUGEPAGE		15
#define	MADV_DONTDUMP		16
#define	MADV_DODUMP		17
#define	MADV_WIPEONFORK		18
#define	MADV_KEEPONFORK		19
#define	MADV_COLD		20
#define	MADV_PAGEOUT		21

#define	MREMAP_MAYMOVE		1
#define	MREMAP_FIXED		2
#define	MREMAP_DONTUNMAP	4

#define	MLOCK_ONFAULT		0x01
