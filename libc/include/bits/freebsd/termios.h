/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Terminal settings as the FreeBSD kernel expects them, from FreeBSD's
 * <sys/_termios.h> and <termios.h>.  Speeds are plain baud rates, so B9600
 * is 9600, and struct termios carries c_ispeed and c_ospeed.  The ioctls
 * are in <bits/freebsd/ttycom.h>.  abi_check.c compares all of this with
 * the FreeBSD headers.
 */

#pragma once

#include <sys/cdefs.h>

/* Indexes into c_cc[]. */
#define	VEOF		0
#define	VEOL		1
#define	VEOL2		2
#define	VERASE		3
#define	VWERASE		4
#define	VKILL		5
#define	VREPRINT	6
#define	VERASE2		7
#define	VINTR		8
#define	VQUIT		9
#define	VSUSP		10
#define	VDSUSP		11
#define	VSTART		12
#define	VSTOP		13
#define	VLNEXT		14
#define	VDISCARD	15
#define	VMIN		16
#define	VTIME		17
#define	VSTATUS		18
#define	NCCS		20

#define	_POSIX_VDISABLE	0xff

/* c_iflag. */
#define	IGNBRK		0x00000001
#define	BRKINT		0x00000002
#define	IGNPAR		0x00000004
#define	PARMRK		0x00000008
#define	INPCK		0x00000010
#define	ISTRIP		0x00000020
#define	INLCR		0x00000040
#define	IGNCR		0x00000080
#define	ICRNL		0x00000100
#define	IXON		0x00000200
#define	IXOFF		0x00000400
#define	IXANY		0x00000800
#define	IMAXBEL		0x00002000
#define	IUTF8		0x00004000

/* c_oflag. */
#define	OPOST		0x00000001
#define	ONLCR		0x00000002
#define	TABDLY		0x00000004
#define	TAB0		0x00000000
#define	TAB3		0x00000004
#define	ONOEOT		0x00000008
#define	OCRNL		0x00000010
#define	ONOCR		0x00000020
#define	ONLRET		0x00000040

/* c_cflag. */
#define	CIGNORE		0x00000001
#define	CSIZE		0x00000300
#define	CS5		0x00000000
#define	CS6		0x00000100
#define	CS7		0x00000200
#define	CS8		0x00000300
#define	CSTOPB		0x00000400
#define	CREAD		0x00000800
#define	PARENB		0x00001000
#define	PARODD		0x00002000
#define	HUPCL		0x00004000
#define	CLOCAL		0x00008000
#define	CCTS_OFLOW	0x00010000
#define	CRTS_IFLOW	0x00020000
#define	CRTSCTS		(CCTS_OFLOW | CRTS_IFLOW)
#define	CDTR_IFLOW	0x00040000
#define	CDSR_OFLOW	0x00080000
#define	CCAR_OFLOW	0x00100000
#define	CNO_RTSDTR	0x00200000

/* c_lflag. */
#define	ECHOKE		0x00000001
#define	ECHOE		0x00000002
#define	ECHOK		0x00000004
#define	ECHO		0x00000008
#define	ECHONL		0x00000010
#define	ECHOPRT		0x00000020
#define	ECHOCTL		0x00000040
#define	ISIG		0x00000080
#define	ICANON		0x00000100
#define	ALTWERASE	0x00000200
#define	IEXTEN		0x00000400
#define	EXTPROC		0x00000800
#define	TOSTOP		0x00400000
#define	FLUSHO		0x00800000
#define	NOKERNINFO	0x02000000
#define	PENDIN		0x20000000
#define	NOFLSH		0x80000000

/* Speeds are baud rates. */
#define	B0		0
#define	B50		50
#define	B75		75
#define	B110		110
#define	B134		134
#define	B150		150
#define	B200		200
#define	B300		300
#define	B600		600
#define	B1200		1200
#define	B1800		1800
#define	B2400		2400
#define	B4800		4800
#define	B7200		7200
#define	B9600		9600
#define	B14400		14400
#define	B19200		19200
#define	B28800		28800
#define	B38400		38400
#define	B57600		57600
#define	B76800		76800
#define	B115200		115200
#define	B230400		230400
#define	B460800		460800
#define	B500000		500000
#define	B921600		921600
#define	B1000000	1000000U
#define	B1500000	1500000U
#define	B2000000	2000000U
#define	B2500000	2500000U
#define	B3000000	3000000U
#define	B3500000	3500000U
#define	B4000000	4000000U
#define	EXTA		19200
#define	EXTB		38400

/* tcsetattr() actions. */
#define	TCSANOW		0
#define	TCSADRAIN	1
#define	TCSAFLUSH	2

/* tcflush() queues and tcflow() actions. */
#define	TCIFLUSH	1
#define	TCOFLUSH	2
#define	TCIOFLUSH	3
#define	TCOOFF		1
#define	TCOON		2
#define	TCIOFF		3
#define	TCION		4

typedef unsigned int	tcflag_t;
typedef unsigned char	cc_t;
typedef unsigned int	speed_t;

struct termios {
	tcflag_t	c_iflag;
	tcflag_t	c_oflag;
	tcflag_t	c_cflag;
	tcflag_t	c_lflag;
	cc_t		c_cc[NCCS];
	speed_t		c_ispeed;
	speed_t		c_ospeed;
};
