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
 * The f_type values for <linux/magic.h>.  statfs() reports these Linux
 * magic numbers for the FreeBSD file system types, as FreeBSD's Linux
 * emulation does (sys/compat/linux/linux_stats.c).  RAMFS_MAGIC is the
 * value of statfs(2); no FreeBSD file system reports it.
 */

#pragma once

#define	CODA_SUPER_MAGIC	0x73757245
#define	EXT2_SUPER_MAGIC	0xEF53
#define	HPFS_SUPER_MAGIC	0xF995E849
#define	ISOFS_SUPER_MAGIC	0x9660
#define	MSDOS_SUPER_MAGIC	0x4D44
#define	NCP_SUPER_MAGIC		0x564C
#define	NFS_SUPER_MAGIC		0x6969
#define	NTFS_SB_MAGIC		0x5346544E
#define	PROC_SUPER_MAGIC	0x9FA0
#define	RAMFS_MAGIC		0x858458f6
#define	SYSFS_MAGIC		0x62656572
#define	TMPFS_MAGIC		0x01021994
