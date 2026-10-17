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
 * uname(3), sethostname(3) and setdomainname(3) on top of sysctl.  FreeBSD
 * has no uname system call; its own libc reads the same sysctls.
 * gethostname() and getdomainname() stay in gethostname.cpp and call
 * uname().
 *
 * field	sysctl name
 * sysname	kern.ostype
 * nodename	kern.hostname
 * release	kern.osrelease
 * version	kern.version
 * machine	hw.machine
 * domainname	kern.nisdomainname
 */

#include <errno.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

/* From FreeBSD's <sys/sysctl.h>. */
#define	CTL_KERN		1
#define	CTL_HW			6
#define	KERN_OSTYPE		1
#define	KERN_OSRELEASE		2
#define	KERN_VERSION		4
#define	KERN_HOSTNAME		10
#define	KERN_NISDOMAINNAME	22
#define	HW_MACHINE		1

int	__sysctl(const int *, unsigned int, void *, size_t *, const void *,
	    size_t);

/*
 * Read one string into a SYS_NMLN field.  The kernel copies what fits of
 * a longer value and fails with ENOMEM, which is fine here.
 */
static int
get_field(int mib0, int mib1, char *field)
{
	int mib[2];
	size_t len;

	mib[0] = mib0;
	mib[1] = mib1;
	len = SYS_NMLN;
	if (__sysctl(mib, 2, field, &len, NULL, 0) != 0 && errno != ENOMEM)
		return (-1);
	field[SYS_NMLN - 1] = '\0';
	return (0);
}

/*
 * Linux rejects names longer than 64 bytes with EINVAL; do the same.
 */
static int
set_name(int mib1, const char *name, size_t len)
{
	int mib[2];

	if (len > SYS_NMLN - 1) {
		errno = EINVAL;
		return (-1);
	}
	mib[0] = CTL_KERN;
	mib[1] = mib1;
	return (__sysctl(mib, 2, NULL, NULL, name, len));
}

int
uname(struct utsname *buf)
{
	memset(buf, 0, sizeof(*buf));
	if (get_field(CTL_KERN, KERN_OSTYPE, buf->sysname) != 0 ||
	    get_field(CTL_KERN, KERN_HOSTNAME, buf->nodename) != 0 ||
	    get_field(CTL_KERN, KERN_OSRELEASE, buf->release) != 0 ||
	    get_field(CTL_KERN, KERN_VERSION, buf->version) != 0 ||
	    get_field(CTL_HW, HW_MACHINE, buf->machine) != 0 ||
	    get_field(CTL_KERN, KERN_NISDOMAINNAME, buf->domainname) != 0)
		return (-1);
	return (0);
}

int
sethostname(const char *name, size_t len)
{
	return (set_name(KERN_HOSTNAME, name, len));
}

int
setdomainname(const char *name, size_t len)
{
	return (set_name(KERN_NISDOMAINNAME, name, len));
}
