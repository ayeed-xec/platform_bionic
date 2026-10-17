/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Terminal window size of the FreeBSD kernel ABI, from FreeBSD
 * sys/sys/_winsize.h and sys/sys/ttycom.h.  struct winsize has the same
 * layout as on Linux; the ioctl numbers are the FreeBSD ones.  The other
 * terminal ioctls come with the termios headers (M1).
 * abi_check.c checks them.
 */

#pragma once

#include <bits/freebsd/ioccom.h>

struct winsize {
	unsigned short	ws_row;
	unsigned short	ws_col;
	unsigned short	ws_xpixel;
	unsigned short	ws_ypixel;
};

#define	TIOCSWINSZ	_IOW('t', 103, struct winsize)
#define	TIOCGWINSZ	_IOR('t', 104, struct winsize)
