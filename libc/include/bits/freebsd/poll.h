/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * poll(2) structure and events of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/poll.h.  POLLWRNORM, POLLWRBAND and POLLRDHUP have other values
 * than on Linux, and FreeBSD has no POLLMSG.  abi_check.c checks them.
 */

#pragma once

struct pollfd {
	int	fd;
	short	events;
	short	revents;
};

#define	POLLIN		0x0001
#define	POLLPRI		0x0002
#define	POLLOUT		0x0004
#define	POLLERR		0x0008
#define	POLLHUP		0x0010
#define	POLLNVAL	0x0020
#define	POLLRDNORM	0x0040
#define	POLLWRNORM	POLLOUT
#define	POLLRDBAND	0x0080
#define	POLLWRBAND	0x0100
#define	POLLRDHUP	0x4000
