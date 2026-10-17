/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
