/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * bionic's readdir() reads directories through __getdents64().  FreeBSD
 * has getdirentries(2) instead, which fills in the FreeBSD struct dirent
 * that <dirent.h> declares.  bionic never needs the base offset, so pass
 * NULL for it.
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
