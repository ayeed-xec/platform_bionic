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
 * sysinfo(2) for bionic-freebsd.  FreeBSD has no sysinfo(2).  The vm.sysinfo
 * sysctl gives the load averages and the memory, swap and process counts in
 * one read, and CLOCK_UPTIME comes from the timekeep page.  The rules are
 * those of FreeBSD's Linux emulation (sys/compat/linux/linux_misc.c,
 * linux_sysinfo()): the uptime is rounded up, the load averages are scaled
 * by 1 << SI_LOAD_SHIFT, and FreeBSD has no buffer, shared or high memory
 * counts.
 */

#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>

/* From FreeBSD's <sys/sysctl.h> and <vm/vm_param.h>. */
#define	CTL_VM		2
#define	VM_SYSINFO	13

/* FreeBSD <sys/vmmeter.h>. */
struct vmsysinfo {
	uint64_t	vs_physmem;
	uint64_t	vs_free;
	uint64_t	vs_swaptotal;
	uint64_t	vs_swapfree;
	uint32_t	vs_ldavg[3];
	uint32_t	vs_fscale;
	uint32_t	vs_nprocs;
	uint32_t	vs_spare[3];
};

int	__sysctl(const int *, unsigned int, void *, size_t *, const void *,
	    size_t);

int
sysinfo(struct sysinfo *info)
{
	struct vmsysinfo vs;
	struct timespec ts;
	size_t len;
	int i, mib[2];

	mib[0] = CTL_VM;
	mib[1] = VM_SYSINFO;
	len = sizeof(vs);
	if (clock_gettime(CLOCK_UPTIME, &ts) != 0 ||
	    __sysctl(mib, 2, &vs, &len, NULL, 0) != 0)
		return (-1);

	memset(info, 0, sizeof(*info));
	info->uptime = ts.tv_sec + (ts.tv_nsec != 0 ? 1 : 0);
	for (i = 0; i < 3; i++)
		info->loads[i] = (unsigned long)vs.vs_ldavg[i] *
		    (1UL << SI_LOAD_SHIFT) / vs.vs_fscale;
	info->totalram = vs.vs_physmem;
	info->freeram = vs.vs_free;
	info->totalswap = vs.vs_swaptotal;
	info->freeswap = vs.vs_swapfree;
	info->procs = vs.vs_nprocs > USHRT_MAX ? USHRT_MAX : vs.vs_nprocs;
	info->mem_unit = 1;
	return (0);
}
