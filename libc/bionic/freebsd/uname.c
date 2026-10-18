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
 * has no uname system call.  The kern.utsname sysctl gives all the fields
 * in one read, each with the value of its own sysctl.
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
#define	KERN_HOSTNAME		10
#define	KERN_NISDOMAINNAME	22
#define	KERN_UTSNAME		40

/* FreeBSD <sys/utsname.h>. */
#define	XUTSNAMELEN		256

struct xutsname {
	char	xu_sysname[XUTSNAMELEN];
	char	xu_nodename[XUTSNAMELEN];
	char	xu_release[XUTSNAMELEN];
	char	xu_version[XUTSNAMELEN];
	char	xu_machine[XUTSNAMELEN];
	char	xu_domainname[XUTSNAMELEN];
};

int	__sysctl(const int *, unsigned int, void *, size_t *, const void *,
	    size_t);

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
	struct xutsname xu;
	size_t len;
	int mib[2];

	mib[0] = CTL_KERN;
	mib[1] = KERN_UTSNAME;
	len = sizeof(xu);
	if (__sysctl(mib, 2, &xu, &len, NULL, 0) != 0)
		return (-1);
	/* The fields stay zero-filled; a longer value is cut. */
	memset(buf, 0, sizeof(*buf));
	strlcpy(buf->sysname, xu.xu_sysname, sizeof(buf->sysname));
	strlcpy(buf->nodename, xu.xu_nodename, sizeof(buf->nodename));
	strlcpy(buf->release, xu.xu_release, sizeof(buf->release));
	strlcpy(buf->version, xu.xu_version, sizeof(buf->version));
	strlcpy(buf->machine, xu.xu_machine, sizeof(buf->machine));
	strlcpy(buf->domainname, xu.xu_domainname, sizeof(buf->domainname));
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
