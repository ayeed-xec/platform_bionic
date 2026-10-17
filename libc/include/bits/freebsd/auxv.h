/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
