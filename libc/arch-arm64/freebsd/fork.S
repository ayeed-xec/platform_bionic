/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Raw fork(2) for __clone_for_fork().  The kernel sets x1 to 1 in the
 * child; clear x0 there so the child sees 0, as vfork.S does.
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
