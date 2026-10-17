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

/*
 * ioctl(2) request encoding of the FreeBSD kernel, from FreeBSD
 * sys/sys/ioccom.h: a 13-bit size and the IOC_VOID, IOC_OUT and IOC_IN
 * direction bits.  abi_check.c checks the values.
 */

#pragma once

#define	IOCPARM_SHIFT	13
#define	IOCPARM_MASK	((1 << IOCPARM_SHIFT) - 1)
#define	IOCPARM_LEN(x)	(((x) >> 16) & IOCPARM_MASK)
#define	IOCBASECMD(x)	((x) & ~(IOCPARM_MASK << 16))
#define	IOCGROUP(x)	(((x) >> 8) & 0xff)
#define	IOCPARM_MAX	(1 << IOCPARM_SHIFT)

#define	IOC_VOID	0x20000000UL
#define	IOC_OUT		0x40000000UL
#define	IOC_IN		0x80000000UL
#define	IOC_INOUT	(IOC_IN | IOC_OUT)
#define	IOC_DIRMASK	(IOC_VOID | IOC_OUT | IOC_IN)

#define	_IOC(inout, group, num, len)	((unsigned long) \
	((inout) | (((len) & IOCPARM_MASK) << 16) | ((group) << 8) | (num)))
#define	_IO(g, n)	_IOC(IOC_VOID, (g), (n), 0)
#define	_IOWINT(g, n)	_IOC(IOC_VOID, (g), (n), sizeof(int))
#define	_IOR(g, n, t)	_IOC(IOC_OUT, (g), (n), sizeof(t))
#define	_IOW(g, n, t)	_IOC(IOC_IN, (g), (n), sizeof(t))
#define	_IOWR(g, n, t)	_IOC(IOC_INOUT, (g), (n), sizeof(t))
