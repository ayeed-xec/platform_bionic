/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * statfs(2) and fstatfs(2) with Android's struct statfs (ADR 0002, decision
 * 1.3).  The kernel fills in FreeBSD's struct statfs.  f_type gets the Linux
 * magic number of the file system type name, and f_flags gets the Linux ST_*
 * flags, as in FreeBSD's Linux emulation (sys/compat/linux/linux_stats.c).
 * An unknown file system type gives f_type 0.
 */

#include <string.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>

#include <bits/freebsd/statfs.h>

/* Linux sets this flag in f_flags to show that the field is valid. */
#define	ST_VALID		0x0020

/* The magic numbers that <sys/vfs.h> does not name. */
#define	UFS_MAGIC		0x00011954
#define	ZFS_SUPER_MAGIC		0x2fc12fc1

int	__statfs(const char *, struct __freebsd_statfs *);
int	__fstatfs(int, struct __freebsd_statfs *);

static const struct {
	const char	*name;
	uint64_t	 magic;
} fs_types[] = {
	{ "cd9660",	ISOFS_SUPER_MAGIC },
	{ "coda",	CODA_SUPER_MAGIC },
	{ "devfs",	DEVFS_SUPER_MAGIC },
	{ "ext2fs",	EXT2_SUPER_MAGIC },
	{ "hpfs",	HPFS_SUPER_MAGIC },
	{ "msdosfs",	MSDOS_SUPER_MAGIC },
	{ "nfs",	NFS_SUPER_MAGIC },
	{ "ntfs",	NTFS_SB_MAGIC },
	{ "nwfs",	NCP_SUPER_MAGIC },
	{ "procfs",	PROC_SUPER_MAGIC },
	{ "tmpfs",	TMPFS_MAGIC },
	{ "ufs",	UFS_MAGIC },
	{ "zfs",	ZFS_SUPER_MAGIC },
};

static void
convert(const struct __freebsd_statfs *src, struct statfs *dst)
{
	size_t i;

	memset(dst, 0, sizeof(*dst));
	for (i = 0; i < sizeof(fs_types) / sizeof(fs_types[0]); i++) {
		if (strcmp(src->f_fstypename, fs_types[i].name) == 0) {
			dst->f_type = fs_types[i].magic;
			break;
		}
	}
	dst->f_bsize = src->f_bsize;
	dst->f_frsize = src->f_bsize;
	dst->f_blocks = src->f_blocks;
	dst->f_bfree = src->f_bfree;
	dst->f_bavail = src->f_bavail < 0 ? 0 : src->f_bavail;
	dst->f_files = src->f_files;
	dst->f_ffree = src->f_ffree < 0 ? 0 : src->f_ffree;
	dst->f_fsid.__val[0] = src->f_fsid[0];
	dst->f_fsid.__val[1] = src->f_fsid[1];
	dst->f_namelen = src->f_namemax;
	dst->f_flags = ST_VALID;
	if (src->f_flags & __FREEBSD_MNT_RDONLY)
		dst->f_flags |= ST_RDONLY;
	if (src->f_flags & __FREEBSD_MNT_NOSUID)
		dst->f_flags |= ST_NOSUID;
	if (src->f_flags & __FREEBSD_MNT_NOEXEC)
		dst->f_flags |= ST_NOEXEC;
	if (src->f_flags & __FREEBSD_MNT_SYNCHRONOUS)
		dst->f_flags |= ST_SYNCHRONOUS;
	if (src->f_flags & __FREEBSD_MNT_NOATIME)
		dst->f_flags |= ST_NOATIME;
	if (src->f_flags & __FREEBSD_MNT_NOSYMFOLLOW)
		dst->f_flags |= ST_NOSYMFOLLOW;
}

int
statfs(const char *path, struct statfs *buf)
{
	struct __freebsd_statfs fs;

	if (__statfs(path, &fs) != 0)
		return (-1);
	convert(&fs, buf);
	return (0);
}

int
fstatfs(int fd, struct statfs *buf)
{
	struct __freebsd_statfs fs;

	if (__fstatfs(fd, &fs) != 0)
		return (-1);
	convert(&fs, buf);
	return (0);
}

/* On LP64, struct statfs64 is struct statfs. */
int
statfs64(const char *path, struct statfs64 *buf)
{
	return (statfs(path, (struct statfs *)buf));
}

int
fstatfs64(int fd, struct statfs64 *buf)
{
	return (fstatfs(fd, (struct statfs *)buf));
}
