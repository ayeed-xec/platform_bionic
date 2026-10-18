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
 * The securebits of Linux, for <linux/securebits.h>: the bit numbers and
 * the masks of prctl(PR_GET_SECUREBITS) and prctl(PR_SET_SECUREBITS).
 * FreeBSD has no securebits; libcap uses the names.
 */

#pragma once

#define	issecure_mask(x)	(1 << (x))

#define	SECURE_NOROOT				0
#define	SECURE_NOROOT_LOCKED			1
#define	SECURE_NO_SETUID_FIXUP			2
#define	SECURE_NO_SETUID_FIXUP_LOCKED		3
#define	SECURE_KEEP_CAPS			4
#define	SECURE_KEEP_CAPS_LOCKED			5
#define	SECURE_NO_CAP_AMBIENT_RAISE		6
#define	SECURE_NO_CAP_AMBIENT_RAISE_LOCKED	7

#define	SECBIT_NOROOT		issecure_mask(SECURE_NOROOT)
#define	SECBIT_NOROOT_LOCKED	issecure_mask(SECURE_NOROOT_LOCKED)
#define	SECBIT_NO_SETUID_FIXUP	issecure_mask(SECURE_NO_SETUID_FIXUP)
#define	SECBIT_NO_SETUID_FIXUP_LOCKED \
	issecure_mask(SECURE_NO_SETUID_FIXUP_LOCKED)
#define	SECBIT_KEEP_CAPS	issecure_mask(SECURE_KEEP_CAPS)
#define	SECBIT_KEEP_CAPS_LOCKED	issecure_mask(SECURE_KEEP_CAPS_LOCKED)
#define	SECBIT_NO_CAP_AMBIENT_RAISE \
	issecure_mask(SECURE_NO_CAP_AMBIENT_RAISE)
#define	SECBIT_NO_CAP_AMBIENT_RAISE_LOCKED \
	issecure_mask(SECURE_NO_CAP_AMBIENT_RAISE_LOCKED)
