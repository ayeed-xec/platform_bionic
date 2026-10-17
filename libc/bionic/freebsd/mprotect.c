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
 * mprotect(2) with Android's PROT_BTI on arm64.  The FreeBSD kernel does not
 * take PROT_BTI.  It marks pages as BTI guarded pages with sysarch(2)
 * ARM64_GUARD_PAGE instead, as FreeBSD's rtld does for objects with the BTI
 * property.  The mark cannot be removed, but it has an effect only on
 * executable pages.  Callers use PROT_BTI only when AT_HWCAP2 has HWCAP2_BTI.
 */

#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

/* FreeBSD <machine/sysarch.h>. */
#define	ARM64_GUARD_PAGE	0x100

struct arm64_guard_page_args {
	uintptr_t	addr;
	size_t		len;
};

int	__mprotect(void *, size_t, int);
int	__sysarch(int, void *);

int
mprotect(void *addr, size_t len, int prot)
{
	struct arm64_guard_page_args guard;

	if ((prot & PROT_BTI) == 0)
		return (__mprotect(addr, len, prot));
	if (__mprotect(addr, len, prot & ~PROT_BTI) != 0)
		return (-1);
	guard.addr = (uintptr_t)addr;
	guard.len = len;
	return (__sysarch(ARM64_GUARD_PAGE, &guard));
}
