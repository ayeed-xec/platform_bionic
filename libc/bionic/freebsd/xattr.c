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
 * The xattr calls of bionic-freebsd on FreeBSD extattr(2).  Each call is one
 * extattr call.  A Linux name has a name space prefix:
 *
 * "user.N"	the user name space, attribute N
 * "security.N", "trusted.N", "system.N"
 *		the system name space, attribute with the full name, so that
 *		"security.N" and "trusted.N" stay two attributes
 *
 * Another prefix fails with ENOTSUP, as on Linux.  FreeBSD has no ENOATTR in
 * the Android errno numbers; the system call stubs give ENODATA for it.
 *
 * Not in one call, so not here (recorded gaps):
 * XATTR_CREATE, XATTR_REPLACE	extattr_set has no flags; a check before the
 *				set is a second call and a race.  ENOTSUP.
 * ERANGE			extattr_get copies what fits and returns its
 *				length, as read(2) does.
 * listxattr			a list is one call for each name space, and
 *				the names need a new format.  ENOTSUP.
 */

#include <sys/types.h>
#include <sys/xattr.h>

#include <errno.h>
#include <string.h>

/* FreeBSD <sys/extattr.h>. */
#define	EXTATTR_NAMESPACE_USER		1
#define	EXTATTR_NAMESPACE_SYSTEM	2

ssize_t	__extattr_get_fd(int, int, const char *, void *, size_t);
ssize_t	__extattr_get_file(const char *, int, const char *, void *, size_t);
ssize_t	__extattr_get_link(const char *, int, const char *, void *, size_t);
ssize_t	__extattr_set_fd(int, int, const char *, const void *, size_t);
ssize_t	__extattr_set_file(const char *, int, const char *, const void *,
	    size_t);
ssize_t	__extattr_set_link(const char *, int, const char *, const void *,
	    size_t);
int	__extattr_delete_fd(int, int, const char *);
int	__extattr_delete_file(const char *, int, const char *);
int	__extattr_delete_link(const char *, int, const char *);

/*
 * Split a Linux attribute name into a name space and a FreeBSD name.
 */
static int
xattr_name(const char *name, int *ns, const char **attr)
{
	static const char user[] = "user.";

	if (strncmp(name, user, sizeof(user) - 1) == 0) {
		*ns = EXTATTR_NAMESPACE_USER;
		*attr = name + sizeof(user) - 1;
		return (0);
	}
	if (strncmp(name, "security.", 9) == 0 ||
	    strncmp(name, "trusted.", 8) == 0 ||
	    strncmp(name, "system.", 7) == 0) {
		*ns = EXTATTR_NAMESPACE_SYSTEM;
		*attr = name;
		return (0);
	}
	errno = ENOTSUP;
	return (-1);
}

/*
 * Only flags 0 (create or replace) is one extattr call.
 */
static int
xattr_flags(int flags)
{
	if (flags == 0)
		return (0);
	errno = (flags & ~(XATTR_CREATE | XATTR_REPLACE)) != 0 ? EINVAL :
	    ENOTSUP;
	return (-1);
}

/*
 * Linux returns the size of the value for a size of 0.  FreeBSD does that
 * for a NULL buffer only.
 */
static void *
xattr_buf(void *value, size_t size)
{
	return (size == 0 ? NULL : value);
}

ssize_t
getxattr(const char *path, const char *name, void *value, size_t size)
{
	const char *attr;
	int ns;

	if (xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_get_file(path, ns, attr, xattr_buf(value, size),
	    size));
}

ssize_t
lgetxattr(const char *path, const char *name, void *value, size_t size)
{
	const char *attr;
	int ns;

	if (xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_get_link(path, ns, attr, xattr_buf(value, size),
	    size));
}

ssize_t
fgetxattr(int fd, const char *name, void *value, size_t size)
{
	const char *attr;
	int ns;

	if (xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_get_fd(fd, ns, attr, xattr_buf(value, size), size));
}

int
setxattr(const char *path, const char *name, const void *value, size_t size,
    int flags)
{
	const char *attr;
	int ns;

	if (xattr_flags(flags) != 0 || xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_set_file(path, ns, attr, value, size) == -1 ? -1 : 0);
}

int
lsetxattr(const char *path, const char *name, const void *value, size_t size,
    int flags)
{
	const char *attr;
	int ns;

	if (xattr_flags(flags) != 0 || xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_set_link(path, ns, attr, value, size) == -1 ? -1 : 0);
}

int
fsetxattr(int fd, const char *name, const void *value, size_t size,
    int flags)
{
	const char *attr;
	int ns;

	if (xattr_flags(flags) != 0 || xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_set_fd(fd, ns, attr, value, size) == -1 ? -1 : 0);
}

int
removexattr(const char *path, const char *name)
{
	const char *attr;
	int ns;

	if (xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_delete_file(path, ns, attr));
}

int
lremovexattr(const char *path, const char *name)
{
	const char *attr;
	int ns;

	if (xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_delete_link(path, ns, attr));
}

int
fremovexattr(int fd, const char *name)
{
	const char *attr;
	int ns;

	if (xattr_name(name, &ns, &attr) != 0)
		return (-1);
	return (__extattr_delete_fd(fd, ns, attr));
}

ssize_t
listxattr(const char *path, char *list, size_t size)
{
	(void)path;
	(void)list;
	(void)size;
	errno = ENOTSUP;
	return (-1);
}

ssize_t
llistxattr(const char *path, char *list, size_t size)
{
	(void)path;
	(void)list;
	(void)size;
	errno = ENOTSUP;
	return (-1);
}

ssize_t
flistxattr(int fd, char *list, size_t size)
{
	(void)fd;
	(void)list;
	(void)size;
	errno = ENOTSUP;
	return (-1);
}
