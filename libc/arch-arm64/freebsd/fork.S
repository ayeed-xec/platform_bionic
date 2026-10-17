/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * fork(2) for __clone_for_fork().  In the child, the FreeBSD kernel returns
 * 1 in x1, and x0 is then made 0, as in vfork.S.
 */

#include <private/bionic_asm.h>

ENTRY_PRIVATE(__freebsd_fork)
	mov	x8, SYS_fork
	svc	#0
	b.cs	__set_errno_from_freebsd
	cbz	x1, 1f
	mov	x0, #0
1:
	ret
END(__freebsd_fork)

NOTE_GNU_PROPERTY()
