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
 * The capability interface of Linux, for Android code: the structures of
 * capget(2) and capset(2), the capability numbers and their bit macros
 * (docs/specs/linux-uapi-headers.md).  The numbers are the Linux numbers,
 * so that Android data such as the file capabilities of fs_config keeps
 * its meaning.  The file capability format (struct vfs_cap_data) is not
 * here yet.
 */

#pragma once

#include <stdint.h>

#define	_LINUX_CAPABILITY_VERSION_1	0x19980330
#define	_LINUX_CAPABILITY_U32S_1	1
#define	_LINUX_CAPABILITY_VERSION_2	0x20071026
#define	_LINUX_CAPABILITY_U32S_2	2
#define	_LINUX_CAPABILITY_VERSION_3	0x20080522
#define	_LINUX_CAPABILITY_U32S_3	2

typedef struct __user_cap_header_struct {
	uint32_t	version;
	int		pid;
} *cap_user_header_t;

/* One element for each 32 capabilities: _LINUX_CAPABILITY_U32S_3 of them. */
typedef struct __user_cap_data_struct {
	uint32_t	effective;
	uint32_t	permitted;
	uint32_t	inheritable;
} *cap_user_data_t;

#define	CAP_CHOWN		0
#define	CAP_DAC_OVERRIDE	1
#define	CAP_DAC_READ_SEARCH	2
#define	CAP_FOWNER		3
#define	CAP_FSETID		4
#define	CAP_KILL		5
#define	CAP_SETGID		6
#define	CAP_SETUID		7
#define	CAP_SETPCAP		8
#define	CAP_LINUX_IMMUTABLE	9
#define	CAP_NET_BIND_SERVICE	10
#define	CAP_NET_BROADCAST	11
#define	CAP_NET_ADMIN		12
#define	CAP_NET_RAW		13
#define	CAP_IPC_LOCK		14
#define	CAP_IPC_OWNER		15
#define	CAP_SYS_MODULE		16
#define	CAP_SYS_RAWIO		17
#define	CAP_SYS_CHROOT		18
#define	CAP_SYS_PTRACE		19
#define	CAP_SYS_PACCT		20
#define	CAP_SYS_ADMIN		21
#define	CAP_SYS_BOOT		22
#define	CAP_SYS_NICE		23
#define	CAP_SYS_RESOURCE	24
#define	CAP_SYS_TIME		25
#define	CAP_SYS_TTY_CONFIG	26
#define	CAP_MKNOD		27
#define	CAP_LEASE		28
#define	CAP_AUDIT_WRITE		29
#define	CAP_AUDIT_CONTROL	30
#define	CAP_SETFCAP		31
#define	CAP_MAC_OVERRIDE	32
#define	CAP_MAC_ADMIN		33
#define	CAP_SYSLOG		34
#define	CAP_WAKE_ALARM		35
#define	CAP_BLOCK_SUSPEND	36
#define	CAP_AUDIT_READ		37
#define	CAP_PERFMON		38
#define	CAP_BPF			39
#define	CAP_CHECKPOINT_RESTORE	40

#define	CAP_LAST_CAP		CAP_CHECKPOINT_RESTORE

#define	cap_valid(x)		((x) >= 0 && (x) <= CAP_LAST_CAP)

/* The element and the bit of capability x in the data array. */
#define	CAP_TO_INDEX(x)		((x) >> 5)
#define	CAP_TO_MASK(x)		(1U << ((x) & 31))
