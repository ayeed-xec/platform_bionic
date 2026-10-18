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
#define	KERN_PROC_VMMAP		32
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
