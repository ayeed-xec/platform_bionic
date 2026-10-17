/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
