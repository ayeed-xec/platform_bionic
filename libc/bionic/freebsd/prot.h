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
 * The protection bits that bionic-freebsd gives to mmap(2) and mprotect(2).
 * Android's PROT_BTI is 0x10; FreeBSD reserves 0x10 for CHERI and takes
 * PROT_BTI as 0x40.  The other bits pass unchanged, so that PROT_MTE and
 * PROT_GROWSDOWN still get EINVAL from the kernel.  0x40 is not an Android
 * bit, so it gets EINVAL here, as before.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/mman.h>

#define	__FREEBSD_PROT_BTI	0x40	/* PROT_BTI of FreeBSD */

/* Return the FreeBSD bits, or -1 for a bit that Android does not have. */
static inline int
__freebsd_prot(int prot)
{
	if ((prot & __FREEBSD_PROT_BTI) != 0)
		return (-1);
	if ((prot & PROT_BTI) != 0)
		prot = (prot & ~PROT_BTI) | __FREEBSD_PROT_BTI;
	return (prot);
}
