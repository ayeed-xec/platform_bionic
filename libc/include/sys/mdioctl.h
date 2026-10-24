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
 * @file sys/mdioctl.h
 * @brief FreeBSD's md(4) memory and vnode disks, which the ioctls of
 * /dev/mdctl attach, query, resize and detach.  It is a FreeBSD-only API,
 * not part of the NDK.  abi_check.c compares it with the FreeBSD header.
 */

#include <sys/types.h>

#include <stdint.h>

#include <bits/freebsd/ioccom.h>

enum md_types { MD_MALLOC, MD_PRELOAD, MD_VNODE, MD_SWAP, MD_NULL };

struct md_ioctl {
	unsigned	md_version;
	unsigned	md_unit;
	enum md_types	md_type;
	char		*md_file;
	off_t		md_mediasize;
	unsigned	md_sectorsize;
	unsigned	md_options;
	uint64_t	md_base;
	int		md_fwheads;
	int		md_fwsectors;
	char		*md_label;
	int		md_pad[96];
};

/** The layout version of struct md_ioctl. */
#define	MDIOVERSION	0

#define	MDIOCATTACH	_IOWR('m', 0, struct md_ioctl)
#define	MDIOCDETACH	_IOWR('m', 1, struct md_ioctl)
#define	MDIOCQUERY	_IOWR('m', 2, struct md_ioctl)
#define	MDIOCRESIZE	_IOWR('m', 4, struct md_ioctl)

/** md_options: give the disk the next free unit. */
#define	MD_AUTOUNIT	0x04
/** md_options: the disk is read-only. */
#define	MD_READONLY	0x08
