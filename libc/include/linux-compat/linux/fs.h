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
 * The names of <linux/fs.h> that Android code uses on bionic-freebsd.
 * SEEK_DATA and SEEK_HOLE have the same values on FreeBSD.  The inode flag
 * command has the Linux value: FreeBSD gives ENOTTY for it, as a Linux file
 * system without inode flags does.  The disk commands BLKFLSBUF and
 * BLKROSET also have the Linux values: FreeBSD has no buffer flush and no
 * read-only flag of a disk, and gives ENOTTY for them.  The MS_* flags
 * have the Linux values, which mount() maps to FreeBSD mount options.
 * FIFREEZE and FITHAW have the Linux values: FreeBSD suspends a file system
 * only while the program that asked holds /dev/ufssuspend open, and gives
 * ENOTTY for them.
 */

#pragma once

#include <bits/seek_constants.h>

/*
 * The inode flags of FS_IOC_SETFLAGS, which are the ext2 and ext4 inode
 * flags.  FreeBSD ext2fs has each of them but FS_ENCRYPT_FL and
 * FS_NOCOW_FL.  FreeBSD has its own file flags (chflags(2)).
 */
#define	FS_IOC_SETFLAGS		0x40086602U
#define	FS_SECRM_FL		0x00000001
#define	FS_UNRM_FL		0x00000002
#define	FS_COMPR_FL		0x00000004
#define	FS_SYNC_FL		0x00000008
#define	FS_IMMUTABLE_FL		0x00000010
#define	FS_APPEND_FL		0x00000020
#define	FS_NODUMP_FL		0x00000040
#define	FS_NOATIME_FL		0x00000080
#define	FS_ENCRYPT_FL		0x00000800
#define	FS_INDEX_FL		0x00001000
#define	FS_JOURNAL_DATA_FL	0x00004000
#define	FS_NOTAIL_FL		0x00008000
#define	FS_DIRSYNC_FL		0x00010000
#define	FS_TOPDIR_FL		0x00020000
#define	FS_EXTENT_FL		0x00080000
#define	FS_NOCOW_FL		0x00800000
#define	FS_INLINE_DATA_FL	0x10000000
#define	FS_PROJINHERIT_FL	0x20000000

#define	BLKROSET		0x125d
#define	BLKFLSBUF		0x1261
#define	FIFREEZE		0xc0045877
#define	FITHAW			0xc0045878

#define	MS_RDONLY		0x00000001
#define	MS_NOSUID		0x00000002
#define	MS_NODEV		0x00000004
#define	MS_NOEXEC		0x00000008
#define	MS_SYNCHRONOUS		0x00000010
#define	MS_REMOUNT		0x00000020
#define	MS_NOATIME		0x00000400
#define	MS_NODIRATIME		0x00000800
#define	MS_BIND			0x00001000
#define	MS_MOVE			0x00002000
#define	MS_REC			0x00004000
#define	MS_SILENT		0x00008000
#define	MS_UNBINDABLE		0x00020000
#define	MS_PRIVATE		0x00040000
#define	MS_SLAVE		0x00080000
#define	MS_SHARED		0x00100000
#define	MS_RELATIME		0x00200000
