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
 * Scheduling policies and clone(2) flags.
 *
 * The kernel reads the policies, so they have the FreeBSD values of
 * sys/sys/sched.h.  FreeBSD has no SCHED_BATCH, SCHED_IDLE or
 * SCHED_DEADLINE.  SCHED_RESET_ON_FORK has the Android value; the FreeBSD
 * kernel does not know it, and sched_setscheduler(2) fails with EINVAL if
 * a policy has it.
 *
 * FreeBSD has no clone(2): libc interprets the CLONE_* flags, so they have
 * the Android values (ADR 0002, decision 1.3).
 * abi_check.c checks the FreeBSD values.
 */

#pragma once

#define	SCHED_FIFO		1
#define	SCHED_NORMAL		2
#define	SCHED_RR		3
#define	SCHED_RESET_ON_FORK	0x40000000

#define	CLONE_VM		0x00000100
#define	CLONE_FS		0x00000200
#define	CLONE_FILES		0x00000400
#define	CLONE_SIGHAND		0x00000800
#define	CLONE_PIDFD		0x00001000
#define	CLONE_PTRACE		0x00002000
#define	CLONE_VFORK		0x00004000
#define	CLONE_PARENT		0x00008000
#define	CLONE_THREAD		0x00010000
#define	CLONE_NEWNS		0x00020000
#define	CLONE_SYSVSEM		0x00040000
#define	CLONE_SETTLS		0x00080000
#define	CLONE_PARENT_SETTID	0x00100000
#define	CLONE_CHILD_CLEARTID	0x00200000
#define	CLONE_DETACHED		0x00400000
#define	CLONE_UNTRACED		0x00800000
#define	CLONE_CHILD_SETTID	0x01000000
#define	CLONE_NEWCGROUP		0x02000000
#define	CLONE_NEWUTS		0x04000000
#define	CLONE_NEWIPC		0x08000000
#define	CLONE_NEWUSER		0x10000000
#define	CLONE_NEWPID		0x20000000
#define	CLONE_NEWNET		0x40000000
#define	CLONE_IO		0x80000000

struct clone_args;
struct sched_attr;
