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
 * The names of the Linux audit interface that Android code uses, for
 * <linux/audit.h>.  FreeBSD netlink(4) has no NETLINK_AUDIT family, so a
 * socket of that family fails; the names let the code build.
 */

#pragma once

#include <stdint.h>

/* Message types. */
#define	AUDIT_SET		1001
#define	AUDIT_USER_AVC		1107

/* Bits of audit_status.mask. */
#define	AUDIT_STATUS_PID	0x0004
#define	AUDIT_STATUS_RATE_LIMIT	0x0008

struct audit_status {
	uint32_t	mask;
	uint32_t	enabled;
	uint32_t	failure;
	uint32_t	pid;
	uint32_t	rate_limit;
	uint32_t	backlog_limit;
	uint32_t	lost;
	uint32_t	backlog;
	uint32_t	feature_bitmap;
	uint32_t	backlog_wait_time;
	uint32_t	backlog_wait_time_actual;
};
