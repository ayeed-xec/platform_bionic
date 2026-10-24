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
 * mount(2) on one nmount(2) call.  The Linux flags become options of the
 * FreeBSD file systems, and each part of the data string between commas
 * is one more option, a name or name=value, as mount(8) passes -o.  A bind
 * mount is a nullfs(5) mount of the source directory.  nullfs(5) does not
 * show the mounts below the source, so MS_REC fails with EINVAL, as each
 * other Linux flag that FreeBSD does not have does.  Only devfs(5) has
 * device files, so MS_NODEV changes nothing, and MS_SILENT is only for the
 * kernel log.  nmount(2) needs the FreeBSD name of the file system type,
 * but an update keeps the type of the mount, so MS_REMOUNT takes no type,
 * as on Linux.  The kernel puts the old options of an update in front of
 * the new ones, so an update without MS_RDONLY also gives "noro", as
 * mount(8) does; else "ro" would stay.
 */

#include <sys/types.h>
#include <sys/mount.h>
#include <sys/uio.h>

#include <errno.h>
#include <string.h>

#include "stubs.h"

#define	MOUNT_FLAGS	(MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | \
	    MS_SYNCHRONOUS | MS_REMOUNT | MS_NOATIME | MS_BIND | MS_SILENT)
#define	MOUNT_IOV	64

static int	add_opt(struct iovec *, unsigned int *, const char *,
		    const char *);

static int
add_opt(struct iovec *iov, unsigned int *n, const char *name,
    const char *value)
{
	if (*n + 2 > MOUNT_IOV)
		return (-1);
	iov[*n].iov_base = (void *)name;
	iov[*n].iov_len = strlen(name) + 1;
	iov[*n + 1].iov_base = (void *)value;
	iov[*n + 1].iov_len = value != NULL ? strlen(value) + 1 : 0;
	*n += 2;
	return (0);
}

int
mount(const char *source, const char *target, const char *fstype,
    unsigned long flags, const void *data)
{
	static const struct {
		unsigned long	flag;
		const char	*name;
	} opts[] = {
		{ MS_RDONLY, "ro" },
		{ MS_NOSUID, "nosuid" },
		{ MS_NOEXEC, "noexec" },
		{ MS_SYNCHRONOUS, "sync" },
		{ MS_REMOUNT, "update" },
		{ MS_NOATIME, "noatime" },
	};
	struct iovec iov[MOUNT_IOV];
	char buf[1024], *p, *s, *v;
	unsigned int i, n;

	if ((flags & MS_BIND) != 0)
		fstype = "nullfs";
	else if ((flags & MS_REMOUNT) != 0 && fstype == NULL)
		fstype = "";
	if ((flags & ~MOUNT_FLAGS) != 0 || fstype == NULL ||
	    (*fstype == '\0' && (flags & MS_REMOUNT) == 0))
		goto inval;
	n = 0;
	add_opt(iov, &n, "fstype", fstype);
	add_opt(iov, &n, "fspath", target);
	if (source != NULL && *source != '\0')
		add_opt(iov, &n, (flags & MS_BIND) != 0 ? "target" : "from",
		    source);
	for (i = 0; i < sizeof(opts) / sizeof(opts[0]); i++)
		if ((flags & opts[i].flag) != 0)
			add_opt(iov, &n, opts[i].name, NULL);
	if ((flags & (MS_REMOUNT | MS_RDONLY)) == MS_REMOUNT)
		add_opt(iov, &n, "noro", NULL);
	if (data != NULL) {
		if (strlcpy(buf, data, sizeof(buf)) >= sizeof(buf))
			goto inval;
		for (p = buf; (s = strsep(&p, ",")) != NULL;) {
			if (*s == '\0')
				continue;
			if ((v = strchr(s, '=')) != NULL)
				*v++ = '\0';
			if (add_opt(iov, &n, s, v) != 0)
				goto inval;
		}
	}
	return (__nmount(iov, n, 0));
inval:
	errno = EINVAL;
	return (-1);
}
