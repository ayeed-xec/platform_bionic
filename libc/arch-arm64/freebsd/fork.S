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
 * rfork_ids(2) and process clone stubs.  On arm64 the child comes back with
 * x0 and x1 both 0.  Mask x0 with x1 - 1 as FreeBSD's own stub does, so the
 * child sees 0 even if the kernel puts 1 in x1.
 */

#include <private/bionic_asm.h>

ENTRY_PRIVATE(__freebsd_rfork_ids)
	mov	x8, SYS_rfork_ids
	svc	#0
	b.cs	__set_errno_from_freebsd
	sub	x1, x1, #1
	and	x0, x0, x1
	ret
END(__freebsd_rfork_ids)

/*
 * pid_t __freebsd_clone(int flags, void *child_stack, int (*fn)(void *),
 *     void *arg, int use_vfork, struct rfork_ids *ids, int *child_tid)
 *
 * rfork_ids(2), rfork(2), or vfork(2), for a clone() with a
 * function.  The kernel gives the child the registers of the parent from
 * x2 up, so the child moves to its stack before it touches memory, and then
 * calls __start_thread(fn, arg, ids, child_tid), which does not return.
 */
ENTRY_PRIVATE(__freebsd_clone)
	mov	x9, x1
	mov	x8, SYS_rfork
	cbz	x5, 1f
	mov	x8, SYS_rfork_ids
	mov	x1, x5
1:	cbz	x4, 2f
	mov	x8, SYS_vfork
2:	svc	#0
	b.cs	__set_errno_from_freebsd
	sub	x1, x1, #1
	and	x0, x0, x1
	cbz	x0, 3f
	ret
3:	mov	sp, x9
	/* The end of the frame record chain; the unwinder stops here. */
	mov	fp, #0
	mov	lr, #0
	mov	x0, x2
	mov	x1, x3
	mov	x2, x5
	mov	x3, x6
	b	__start_thread
END(__freebsd_clone)

NOTE_GNU_PROPERTY()
