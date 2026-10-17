/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
