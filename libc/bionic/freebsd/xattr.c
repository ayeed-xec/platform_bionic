/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * The xattr calls of bionic-freebsd.  FreeBSD has extattr(2), not the
 * xattr system calls.  Until libc emulates them on extattr(2) (M1), they
 * fail with ENOTSUP, as on a Linux file system without extended
 * attributes.  bionic calls only fsetxattr(), to put the SELinux label on
 * a new property file, and it continues without the label.
 */

#include <errno.h>
#include <sys/xattr.h>

int
fsetxattr(int fd, const char *name, const void *value, size_t size,
    int flags)
{
	(void)fd;
	(void)name;
	(void)value;
	(void)size;
	(void)flags;
	errno = ENOTSUP;
	return (-1);
}
