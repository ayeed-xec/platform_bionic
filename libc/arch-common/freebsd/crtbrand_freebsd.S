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
 * The FreeBSD ABI tag note.  The kernel reads it to select the native
 * FreeBSD brand and to set the osrel of the process (p_osrel).  The
 * format is that of elf(5) and FreeBSD sys/sys/elf_common.h:
 * NT_FREEBSD_ABI_TAG (1), owner "FreeBSD", desc __FreeBSD_version.
 */

#ifndef FREEBSD_ABI_VERSION
#define	FREEBSD_ABI_VERSION	1501000	/* FreeBSD 15.1, sys/param.h */
#endif

#define	NT_FREEBSD_ABI_TAG	1

/*
 * No section group: lld garbage-collects a note that is in a group, and
 * then the kernel gets no osrel.  Only crtbegin carries this note.
 */
	.section .note.tag,"a",%note
	.p2align 2
	.type	freebsd_abi_tag, %object
freebsd_abi_tag:
	.long	2f - 1f			/* namesz */
	.long	4f - 3f			/* descsz */
	.long	NT_FREEBSD_ABI_TAG	/* type */
1:	.asciz	"FreeBSD"		/* name */
2:	.p2align 2
3:	.long	FREEBSD_ABI_VERSION	/* desc */
4:
	.size	freebsd_abi_tag, . - freebsd_abi_tag

	.section .note.GNU-stack,"",%progbits
