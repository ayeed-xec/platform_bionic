/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * struct iovec of the FreeBSD kernel ABI, from FreeBSD sys/sys/_iovec.h.
 * abi_check.c checks it.
 */

#pragma once

#include <sys/cdefs.h>
#include <stddef.h>

struct iovec {
	void		*iov_base;
	size_t		iov_len;
};

#define	UIO_MAXIOV	1024
