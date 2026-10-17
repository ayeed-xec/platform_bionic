/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * FreeBSD's <libsys.h> declares the raw system call entry points.  bionic-
 * freebsd has the ones that the FreeBSD sources in upstream-freebsd use.
 */

#pragma once

#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

int	__sys___realpathat(int, const char *, char *, size_t, int);

__END_DECLS
