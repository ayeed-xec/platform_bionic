/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

/**
 * @file sys/sysctl.h
 * @brief FreeBSD's sysctl(3) and the kern.proc names.  There is no /proc,
 * so this is how a program reads the state of a process or a thread.  It
 * is a FreeBSD-only API, not part of the NDK.
 */

#include <sys/cdefs.h>
#include <sys/types.h>

#include <bits/freebsd/user.h>

#define	CTL_KERN		1
#define	KERN_PROC		14
#define	KERN_PROC_PID		1
#define	KERN_PROC_INC_THREAD	0x10

__BEGIN_DECLS

/**
 * [sysctl(3)](https://man.freebsd.org/cgi/man.cgi?query=sysctl&sektion=3)
 * reads, and optionally sets, the kernel state that the MIB `name` names.
 *
 * This is the __sysctl system call.  FreeBSD's libc answers the user.*
 * names itself; here the kernel's empty or zero placeholders come back, so use
 * confstr(3) and sysconf(3) for those.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int sysctl(const int* _Nonnull __name, u_int __name_length, void* _Nullable __old, size_t* _Nullable __old_length, const void* _Nullable __new, size_t __new_length);

__END_DECLS
