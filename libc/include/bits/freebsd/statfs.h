/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * The struct statfs of the FreeBSD kernel ABI (FreeBSD sys/sys/mount.h),
 * under another name: the Android API has its own struct statfs in
 * <sys/vfs.h>, and libc converts (ADR 0002, decision 1.3).  abi_check.c
 * checks the layout.
 */

#pragma once

#include <sys/cdefs.h>
#include <stdint.h>

#define	__FREEBSD_MFSNAMELEN		16
#define	__FREEBSD_MNAMELEN		1024

#define	__FREEBSD_MNT_RDONLY		0x0000000000000001ULL
#define	__FREEBSD_MNT_SYNCHRONOUS	0x0000000000000002ULL
#define	__FREEBSD_MNT_NOEXEC		0x0000000000000004ULL
#define	__FREEBSD_MNT_NOSUID		0x0000000000000008ULL
#define	__FREEBSD_MNT_NOSYMFOLLOW	0x0000000000400000ULL
#define	__FREEBSD_MNT_NOATIME		0x0000000010000000ULL

struct __freebsd_statfs {
	uint32_t	f_version;
	uint32_t	f_type;
	uint64_t	f_flags;
	uint64_t	f_bsize;
	uint64_t	f_iosize;
	uint64_t	f_blocks;
	uint64_t	f_bfree;
	int64_t		f_bavail;
	uint64_t	f_files;
	int64_t		f_ffree;
	uint64_t	f_syncwrites;
	uint64_t	f_asyncwrites;
	uint64_t	f_syncreads;
	uint64_t	f_asyncreads;
	uint32_t	f_nvnodelistsize;
	uint32_t	f_spare0;
	uint64_t	f_spare[9];
	uint32_t	f_namemax;
	uint32_t	f_owner;
	int32_t		f_fsid[2];
	char		f_charspare[80];
	char		f_fstypename[__FREEBSD_MFSNAMELEN];
	char		f_mntfromname[__FREEBSD_MNAMELEN];
	char		f_mntonname[__FREEBSD_MNAMELEN];
};
