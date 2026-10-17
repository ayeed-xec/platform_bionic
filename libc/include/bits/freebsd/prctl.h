/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * prctl(2) options.  FreeBSD has no prctl(2): libc emulates it, so the
 * options have the Android values (ADR 0002, decision 1.3).  This file
 * has only the options that bionic uses.
 */

#pragma once

#define	PR_GET_DUMPABLE			3
#define	PR_SET_DUMPABLE			4
#define	PR_SET_NAME			15
#define	PR_GET_NAME			16

#define	PR_PAC_RESET_KEYS		54
#define	PR_PAC_APIAKEY			(1UL << 0)

#define	PR_SET_TAGGED_ADDR_CTRL		55
#define	PR_GET_TAGGED_ADDR_CTRL		56
#define	PR_TAGGED_ADDR_ENABLE		(1UL << 0)
#define	PR_MTE_TCF_SHIFT		1
#define	PR_MTE_TCF_NONE			(0UL << PR_MTE_TCF_SHIFT)
#define	PR_MTE_TCF_SYNC			(1UL << PR_MTE_TCF_SHIFT)
#define	PR_MTE_TCF_ASYNC		(2UL << PR_MTE_TCF_SHIFT)
#define	PR_MTE_TCF_MASK			(3UL << PR_MTE_TCF_SHIFT)
#define	PR_MTE_TAG_SHIFT		3
#define	PR_MTE_TAG_MASK		(0xffffUL << PR_MTE_TAG_SHIFT)

#define	PR_SET_VMA			0x53564d41
#define	PR_SET_VMA_ANON_NAME		0
