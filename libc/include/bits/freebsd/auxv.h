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
 * ELF auxiliary vector types of the FreeBSD kernel, from FreeBSD
 * sys/sys/elf_common.h.  The Linux-only types (AT_SECURE, AT_RANDOM,
 * AT_SYSINFO_EHDR and others) do not exist here; the startup code gets
 * the same data from issetugid(2), AT_CANARY and AT_KPRELOAD (M1).
 * abi_check.c checks the values.
 */

#pragma once

#define	AT_NULL		0
#define	AT_IGNORE	1
#define	AT_EXECFD	2
#define	AT_PHDR		3
#define	AT_PHENT	4
#define	AT_PHNUM	5
#define	AT_PAGESZ	6
#define	AT_BASE		7
#define	AT_FLAGS	8
#define	AT_ENTRY	9
#define	AT_NOTELF	10
#define	AT_UID		11
#define	AT_EUID		12
#define	AT_GID		13
#define	AT_EGID		14
#define	AT_EXECPATH	15
#define	AT_CANARY	16
#define	AT_CANARYLEN	17
#define	AT_OSRELDATE	18
#define	AT_NCPUS	19
#define	AT_PAGESIZES	20
#define	AT_PAGESIZESLEN	21
#define	AT_TIMEKEEP	22
#define	AT_STACKPROT	23
#define	AT_EHDRFLAGS	24
#define	AT_HWCAP	25
#define	AT_HWCAP2	26
#define	AT_BSDFLAGS	27
#define	AT_ARGC		28
#define	AT_ARGV		29
#define	AT_ENVC		30
#define	AT_ENVV		31
#define	AT_PS_STRINGS	32
#define	AT_FXRNG	33
#define	AT_KPRELOAD	34
#define	AT_USRSTACKBASE	35
#define	AT_USRSTACKLIM	36
#define	AT_CHERI_STATS	37
#define	AT_HWCAP3	38
#define	AT_HWCAP4	39
#define	AT_COUNT	40
