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
 * umount(2) and umount2(2) on unmount(2).  MNT_FORCE becomes FreeBSD's
 * MNT_FORCE.  FreeBSD has no lazy unmount, so MNT_DETACH unmounts at once
 * and fails with EBUSY while the file system is in use.  MNT_EXPIRE has no
 * counterpart, and unmount(2) always follows a symbolic link, so these two
 * flags and UMOUNT_NOFOLLOW fail with EINVAL.
 */

#include <sys/mount.h>

#include <errno.h>

/* FreeBSD <sys/mount.h>. */
#define	FREEBSD_MNT_FORCE	0x00080000

int	__unmount(const char *, int);

int
umount2(const char *target, int flags)
{
	int fflags;

	if ((flags & ~(MNT_FORCE | MNT_DETACH)) != 0) {
		errno = EINVAL;
		return (-1);
	}
	fflags = 0;
	if ((flags & MNT_FORCE) != 0)
		fflags |= FREEBSD_MNT_FORCE;
	return (__unmount(target, fflags));
}

int
umount(const char *target)
{
	return (umount2(target, 0));
}
