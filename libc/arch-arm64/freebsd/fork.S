/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Raw fork(2) for __clone_for_fork().  On arm64 the child comes back with
 * x0 and x1 both 0.  Mask x0 with x1 - 1 as FreeBSD's own stub does, so the
 * child sees 0 even if the kernel puts 1 in x1.
 */

#include <private/bionic_asm.h>

ENTRY_PRIVATE(__freebsd_fork)
	mov	x8, SYS_fork
	svc	#0
	b.cs	__set_errno_from_freebsd
	sub	x1, x1, #1
	and	x0, x0, x1
	ret
END(__freebsd_fork)

NOTE_GNU_PROPERTY()
