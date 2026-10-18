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
 * capget(2) and capset(2) for bionic-freebsd.  FreeBSD has no capability
 * sets: a process with effective uid 0 has all the privileges, and another
 * process has none.  capget() gives that state with one geteuid(2) call,
 * and capset() accepts only the requests that keep it.  FreeBSD keeps no
 * sets of other processes, so a pid other than the caller gives EPERM.
 */

#include <sys/types.h>
#include <sys/capability.h>

#include <errno.h>
#include <stdint.h>
#include <unistd.h>

static int
cap_words(cap_user_header_t hdr)
{
	switch (hdr->version) {
	case _LINUX_CAPABILITY_VERSION_1:
		return (_LINUX_CAPABILITY_U32S_1);
	case _LINUX_CAPABILITY_VERSION_2:
	case _LINUX_CAPABILITY_VERSION_3:
		return (_LINUX_CAPABILITY_U32S_3);
	}
	/* The caller learns the version that this libc prefers. */
	hdr->version = _LINUX_CAPABILITY_VERSION_3;
	return (0);
}

/* The bits of all the capabilities in word i of a set. */
static uint32_t
cap_all(int i)
{
	if (i == 0)
		return (UINT32_MAX);
	return (CAP_TO_MASK(CAP_LAST_CAP) | (CAP_TO_MASK(CAP_LAST_CAP) - 1));
}

static int
cap_check(cap_user_header_t hdr, int *nwords)
{
	if (hdr == NULL) {
		errno = EFAULT;
		return (-1);
	}
	*nwords = cap_words(hdr);
	if (*nwords == 0) {
		errno = EINVAL;
		return (-1);
	}
	if (hdr->pid != 0 && hdr->pid != getpid()) {
		errno = EPERM;
		return (-1);
	}
	return (0);
}

int
capget(cap_user_header_t hdr, cap_user_data_t data)
{
	uint32_t set;
	uid_t euid;
	int i, n;

	if (cap_check(hdr, &n) != 0)
		return (-1);
	if (data == NULL)
		return (0);
	euid = geteuid();
	for (i = 0; i < n; i++) {
		set = euid == 0 ? cap_all(i) : 0;
		data[i].effective = set;
		data[i].permitted = set;
		data[i].inheritable = 0;
	}
	return (0);
}

int
capset(cap_user_header_t hdr, const cap_user_data_t data)
{
	uint32_t set;
	uid_t euid;
	int i, n;

	if (cap_check(hdr, &n) != 0)
		return (-1);
	if (data == NULL) {
		errno = EFAULT;
		return (-1);
	}
	euid = geteuid();
	for (i = 0; i < n; i++) {
		set = euid == 0 ? cap_all(i) : 0;
		if (data[i].effective != set || data[i].permitted != set ||
		    data[i].inheritable != 0) {
			errno = EPERM;
			return (-1);
		}
	}
	return (0);
}
