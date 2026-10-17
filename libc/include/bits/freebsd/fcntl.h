/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * open(2), fcntl(2) and *at(2) values of the FreeBSD kernel ABI, from
 * FreeBSD sys/sys/fcntl.h.  abi_check.c checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

#define	O_RDONLY		0x0000
#define	O_WRONLY		0x0001
#define	O_RDWR			0x0002
#define	O_ACCMODE		0x0003
#define	O_NONBLOCK		0x0004
#define	O_NDELAY		O_NONBLOCK
#define	O_APPEND		0x0008
#define	FASYNC			0x0040	/* <fcntl.h>: O_ASYNC */
#define	O_SYNC			0x0080
#define	O_FSYNC			O_SYNC
#define	O_NOFOLLOW		0x0100
#define	O_CREAT			0x0200
#define	O_TRUNC			0x0400
#define	O_EXCL			0x0800
#define	O_NOCTTY		0x8000
#define	O_DIRECT		0x00010000
#define	O_DIRECTORY		0x00020000
#define	O_EXEC			0x00040000
#define	O_SEARCH		O_EXEC
#define	O_CLOEXEC		0x00100000
#define	O_PATH			0x00400000
#define	O_RESOLVE_BENEATH	0x00800000
#define	O_DSYNC			0x01000000
#define	O_EMPTY_PATH		0x02000000
/* Linux names with no FreeBSD flag: the FreeBSD kernel needs none. */
#define	O_LARGEFILE		0
#define	O_NOATIME		0

#define	F_DUPFD			0
#define	F_GETFD			1
#define	F_SETFD			2
#define	F_GETFL			3
#define	F_SETFL			4
#define	F_GETOWN		5
#define	F_SETOWN		6
#define	F_DUP2FD		10
#define	F_GETLK			11
#define	F_SETLK			12
#define	F_SETLKW		13
#define	F_READAHEAD		15
#define	F_RDAHEAD		16
#define	F_DUPFD_CLOEXEC		17
#define	F_DUP2FD_CLOEXEC	18
#define	F_ADD_SEALS		19
#define	F_GET_SEALS		20
#define	F_KINFO			22
#define	F_DUP3FD		24
#define	F_DUP3FD_SHIFT		16

#define	F_SEAL_SEAL		0x0001
#define	F_SEAL_SHRINK		0x0002
#define	F_SEAL_GROW		0x0004
#define	F_SEAL_WRITE		0x0008

#define	FD_CLOEXEC		1

#define	F_RDLCK			1
#define	F_UNLCK			2
#define	F_WRLCK			3

#define	AT_FDCWD		-100
#define	AT_EACCESS		0x0100
#define	AT_SYMLINK_NOFOLLOW	0x0200
#define	AT_SYMLINK_FOLLOW	0x0400
#define	AT_REMOVEDIR		0x0800
#define	AT_RESOLVE_BENEATH	0x2000
#define	AT_EMPTY_PATH		0x4000

#define	POSIX_FADV_NORMAL	0
#define	POSIX_FADV_RANDOM	1
#define	POSIX_FADV_SEQUENTIAL	2
#define	POSIX_FADV_WILLNEED	3
#define	POSIX_FADV_DONTNEED	4
#define	POSIX_FADV_NOREUSE	5

struct flock {
	off_t		l_start;
	off_t		l_len;
	pid_t		l_pid;
	short		l_type;
	short		l_whence;
	int		l_sysid;
};
