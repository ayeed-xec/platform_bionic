/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * __getdents64() for bionic-freebsd.  bionic's readdir() reads a directory
 * with it.  FreeBSD has no getdents64.  getdirentries(2) has the same job,
 * and it writes the FreeBSD struct dirent that <dirent.h> has (ADR 0002).
 * The fourth argument of getdirentries(2) is the base position of the
 * block.  bionic does not use it: the wrapper passes NULL.
 */

#include <dirent.h>
#include <stddef.h>
#include <sys/types.h>

ssize_t	__getdirentries(int, char *, size_t, off_t *);
int	__getdents64(unsigned int, struct dirent *, unsigned int);

int
__getdents64(unsigned int fd, struct dirent *buf, unsigned int count)
{
	return ((int)__getdirentries((int)fd, (char *)buf, count, NULL));
}
