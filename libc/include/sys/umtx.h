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
 * @file sys/umtx.h
 * @brief FreeBSD's _umtx_op(2), the kernel primitive under futex() on
 * bionic-freebsd (ADR 0002, decision 1.3).  It is a FreeBSD-only API, not
 * part of the NDK.  libc++'s atomic wait uses it on FreeBSD.
 */

#include <sys/cdefs.h>
#include <sys/types.h>

#include <bits/freebsd/umtx.h>

__BEGIN_DECLS

/**
 * [_umtx_op(2)](https://man.freebsd.org/cgi/man.cgi?query=_umtx_op&sektion=2)
 * does the operation `op` on the user-space lock or word at `obj`.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int _umtx_op(void* _Nullable __obj, int __op, unsigned long __val, void* _Nullable __uaddr, void* _Nullable __uaddr2);

__END_DECLS
