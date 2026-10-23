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
 * AT_HWCAP and AT_HWCAP2 bits of the FreeBSD arm64 kernel ABI, from
 * FreeBSD sys/arm64/include/elf.h.  The bits have the same values as on
 * Linux.  This file has only the bits that bionic and the AOSP libraries
 * built on it use; zlib tests HWCAP_PMULL and HWCAP_CRC32.
 * abi_check.c checks the values.
 */

#pragma once

#if defined(__aarch64__)
#define	HWCAP_ASIMD		(1 << 1)
#define	HWCAP_PMULL		(1 << 4)
#define	HWCAP_CRC32		(1 << 7)
#define	HWCAP_CPUID		(1 << 11)
#define	HWCAP_PACA		(1 << 30)

#define	HWCAP2_BTI		(1 << 17)
#define	HWCAP2_MTE		(1 << 18)
#define	HWCAP2_SME		(1 << 23)
#define	HWCAP2_MOPS		(1UL << 43)
#endif
