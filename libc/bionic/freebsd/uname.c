/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * uname(2), sethostname(2) and setdomainname(2) for bionic-freebsd.
 * FreeBSD has no uname system call.  The fields of bionic's struct utsname
 * come from sysctl names (__sysctl() with MIB numbers), and the two set
 * functions write the sysctl names kern.hostname and kern.nisdomainname.
 * gethostname() and getdomainname() are in bionic's gethostname.cpp.
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

/* MIB numbers from FreeBSD sys/sys/sysctl.h. */
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
 * Read one string value into a field of SYS_NMLN bytes.  When the value is
 * longer, the kernel copies the first bytes and fails with ENOMEM: cut the
 * value at the field size.
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
 * Write a name.  Linux fails with EINVAL when the name is longer than 64
 * bytes.
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
