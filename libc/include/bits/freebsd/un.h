/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * struct sockaddr_un of the FreeBSD kernel ABI, from FreeBSD sys/sys/un.h.
 * The path is 104 bytes long and the structure has a length byte.
 * abi_check.c checks it.
 */

#pragma once

#include <sys/cdefs.h>
#include <bits/sa_family_t.h>

#define	SUNPATHLEN	104

struct sockaddr_un {
	unsigned char	sun_len;
	sa_family_t	sun_family;
	char		sun_path[SUNPATHLEN];
};
