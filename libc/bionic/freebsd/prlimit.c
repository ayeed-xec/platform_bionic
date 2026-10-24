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
 * prlimit(2) and setrlimit(2) for bionic-freebsd.  The kern.proc.rlimit
 * sysctl of a process reads the old limit and sets the new one in one call.
 * pid 0 is the calling process, as on Linux.  The sysctl copies the old
 * limit out before it reads the new one, so the new limit is copied first:
 * the caller can give one structure for both, as on Linux.
 *
 * Linux refuses a soft limit above the hard limit with EINVAL.  FreeBSD
 * lowers the soft limit to the hard limit for root, and gives EPERM to
 * another user when the soft limit is above the old hard limit, so both
 * functions check it first.
 */

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/sysctl.h>

#include <errno.h>
#include <unistd.h>

#include "stubs.h"

int
prlimit(pid_t pid, int resource, const struct rlimit *new_limit,
    struct rlimit *old_limit)
{
	struct rlimit rl;
	int mib[5];
	size_t len;

	if (new_limit != NULL) {
		if (new_limit->rlim_cur > new_limit->rlim_max) {
			errno = EINVAL;
			return (-1);
		}
		rl = *new_limit;
		new_limit = &rl;
	}
	mib[0] = CTL_KERN;
	mib[1] = KERN_PROC;
	mib[2] = KERN_PROC_RLIMIT;
	mib[3] = pid != 0 ? pid : getpid();
	mib[4] = resource;
	len = sizeof(*old_limit);
	return (sysctl(mib, 5, old_limit, old_limit != NULL ? &len : NULL,
	    new_limit, new_limit != NULL ? sizeof(*new_limit) : 0));
}

int
setrlimit(int resource, const struct rlimit *rlp)
{
	if (rlp != NULL && rlp->rlim_cur > rlp->rlim_max) {
		errno = EINVAL;
		return (-1);
	}
	return (__setrlimit(resource, rlp));
}
__strong_alias(setrlimit64, setrlimit);
