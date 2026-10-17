/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * AT_HWCAP and AT_HWCAP2 bits of the FreeBSD arm64 kernel ABI, from
 * FreeBSD sys/arm64/include/elf.h.  The bits have the same values as on
 * Linux.  This file has only the bits that bionic uses.
 * abi_check.c checks the values.
 */

#pragma once

#if defined(__aarch64__)
#define	HWCAP_ASIMD		(1 << 1)
#define	HWCAP_CPUID		(1 << 11)
#define	HWCAP_PACA		(1 << 30)

#define	HWCAP2_BTI		(1 << 17)
#define	HWCAP2_MTE		(1 << 18)
#define	HWCAP2_SME		(1 << 23)
#define	HWCAP2_MOPS		(1UL << 43)
#endif
