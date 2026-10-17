/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * sysinfo(2) for bionic-freebsd.  FreeBSD has no sysinfo(2).  The values
 * come from sysctl(3) names, with the rules of FreeBSD's Linux emulation
 * (sys/compat/linux/linux_misc.c, linux_sysinfo()): the uptime is rounded
 * up, the load averages are scaled by 1 << SI_LOAD_SHIFT, and FreeBSD has
 * no buffer, shared or high memory counts.  This version does not count
 * the swap space or the processes yet: these fields are 0.
 */

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

/* FreeBSD <sys/resource.h>. */
struct loadavg {
	uint32_t	ldavg[3];
	long		fscale;
};

int	__sysctlbyname(const char *, size_t, void *, size_t *, const void *,
	    size_t);

static int
get_value(const char *name, void *value, size_t len)
{
	size_t got;

	got = len;
	if (__sysctlbyname(name, strlen(name), value, &got, NULL, 0) != 0)
		return (-1);
	if (got != len) {
		errno = EINVAL;
		return (-1);
	}
	return (0);
}

int
sysinfo(struct sysinfo *info)
{
	struct loadavg la;
	struct timespec ts;
	unsigned long physmem;
	unsigned int free_pages;
	int i;

	if (clock_gettime(CLOCK_UPTIME, &ts) != 0 ||
	    get_value("vm.loadavg", &la, sizeof(la)) != 0 ||
	    get_value("hw.physmem", &physmem, sizeof(physmem)) != 0 ||
	    get_value("vm.stats.vm.v_free_count", &free_pages,
	    sizeof(free_pages)) != 0)
		return (-1);

	memset(info, 0, sizeof(*info));
	info->uptime = ts.tv_sec + (ts.tv_nsec != 0 ? 1 : 0);
	for (i = 0; i < 3; i++)
		info->loads[i] = (unsigned long)la.ldavg[i] *
		    (1UL << SI_LOAD_SHIFT) / la.fscale;
	info->totalram = physmem;
	info->freeram = (unsigned long)free_pages * getpagesize();
	info->mem_unit = 1;
	return (0);
}
