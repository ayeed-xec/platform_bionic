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
 * The ashmem ioctl(2) commands of Android, for <linux/ashmem.h>.  FreeBSD
 * has no ashmem device: ioctl() of bionic-freebsd answers these commands on
 * a memfd, with one fcntl(2) for each command.  The command values are the
 * Linux values, so code built against other Android headers works.
 */

#pragma once

#include <stdint.h>

#define	__ASHMEMIOC		0x77
#define	ASHMEM_NAME_LEN		256

/* Results of ASHMEM_PIN. */
#define	ASHMEM_NOT_PURGED	0
#define	ASHMEM_WAS_PURGED	1

/* Results of ASHMEM_GET_PIN_STATUS. */
#define	ASHMEM_IS_UNPINNED	0
#define	ASHMEM_IS_PINNED	1

struct ashmem_pin {
	uint32_t	offset;
	uint32_t	len;
};

#define	ASHMEM_SET_NAME		0x41007701U
#define	ASHMEM_GET_NAME		0x81007702U
#define	ASHMEM_SET_SIZE		0x40087703U
#define	ASHMEM_GET_SIZE		0x00007704U
#define	ASHMEM_SET_PROT_MASK	0x40087705U
#define	ASHMEM_GET_PROT_MASK	0x00007706U
#define	ASHMEM_PIN		0x40087707U
#define	ASHMEM_UNPIN		0x40087708U
#define	ASHMEM_GET_PIN_STATUS	0x00007709U
#define	ASHMEM_PURGE_ALL_CACHES	0x0000770aU
#define	ASHMEM_GET_FILE_ID	0x8008770bU
