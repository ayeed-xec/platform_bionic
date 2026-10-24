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

#include <sys/types.h>

#include <bits/freebsd/if.h>
#include <bits/freebsd/ioccom.h>

/* The socket ioctls of FreeBSD's <sys/sockio.h> that Android code uses. */
#define	SIOCSIFFLAGS	_IOW('i', 16, struct ifreq)
#define	SIOCGIFFLAGS	_IOWR('i', 17, struct ifreq)
#define	SIOCGIFMETRIC	_IOWR('i', 23, struct ifreq)
#define	SIOCSIFMETRIC	_IOW('i', 24, struct ifreq)
#define	SIOCDIFADDR	_IOW('i', 25, struct ifreq)
#define	SIOCGIFINDEX	_IOWR('i', 32, struct ifreq)
#define	SIOCGIFADDR	_IOWR('i', 33, struct ifreq)
#define	SIOCGIFDSTADDR	_IOWR('i', 34, struct ifreq)
#define	SIOCGIFBRDADDR	_IOWR('i', 35, struct ifreq)
#define	SIOCGIFNETMASK	_IOWR('i', 37, struct ifreq)
#define	SIOCSIFNAME	_IOW('i', 40, struct ifreq)
#define	SIOCAIFADDR	_IOW('i', 43, struct ifaliasreq)
#define	SIOCGIFMTU	_IOWR('i', 51, struct ifreq)
#define	SIOCSIFMTU	_IOW('i', 52, struct ifreq)
#define	SIOCSIFLLADDR	_IOW('i', 60, struct ifreq)
#define	SIOCIFDESTROY	_IOW('i', 121, struct ifreq)
#define	SIOCSDRVSPEC	_IOW('i', 123, struct ifdrv)
#define	SIOCGDRVSPEC	_IOWR('i', 123, struct ifdrv)
#define	SIOCIFCREATE2	_IOWR('i', 124, struct ifreq)
