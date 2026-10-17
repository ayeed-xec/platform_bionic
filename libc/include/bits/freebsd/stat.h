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
 * File mode bits and the struct stat of the FreeBSD kernel ABI, from
 * FreeBSD sys/sys/stat.h (the ino64 layout).  abi_check.c checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>
#include <bits/timespec.h>

#define	S_IFMT		0170000
#define	S_IFIFO		0010000
#define	S_IFCHR		0020000
#define	S_IFDIR		0040000
#define	S_IFBLK		0060000
#define	S_IFREG		0100000
#define	S_IFLNK		0120000
#define	S_IFSOCK	0140000
#define	S_IFWHT		0160000

#define	S_ISUID		0004000
#define	S_ISGID		0002000
#define	S_ISVTX		0001000

#define	S_IRWXU		0000700
#define	S_IRUSR		0000400
#define	S_IWUSR		0000200
#define	S_IXUSR		0000100
#define	S_IRWXG		0000070
#define	S_IRGRP		0000040
#define	S_IWGRP		0000020
#define	S_IXGRP		0000010
#define	S_IRWXO		0000007
#define	S_IROTH		0000004
#define	S_IWOTH		0000002
#define	S_IXOTH		0000001

#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define	S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define	S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

#define	UTIME_NOW	-1
#define	UTIME_OMIT	-2

#define	__STAT_BODY \
	dev_t		st_dev; \
	ino_t		st_ino; \
	nlink_t		st_nlink; \
	mode_t		st_mode; \
	__int16_t	st_bsdflags; \
	uid_t		st_uid; \
	gid_t		st_gid; \
	__int32_t	st_padding1; \
	dev_t		st_rdev; \
	struct timespec	st_atim; \
	struct timespec	st_mtim; \
	struct timespec	st_ctim; \
	struct timespec	st_birthtim; \
	off_t		st_size; \
	blkcnt_t	st_blocks; \
	blksize_t	st_blksize; \
	__uint32_t	st_flags; \
	__uint64_t	st_gen; \
	__uint64_t	st_filerev; \
	__uint64_t	st_spare[9];
