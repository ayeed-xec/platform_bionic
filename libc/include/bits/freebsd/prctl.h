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
 * prctl(2) options.  FreeBSD has no prctl(2): libc emulates it, so the
 * options have the Android values (ADR 0002, decision 1.3).  This file
 * has only the options that bionic and the libraries of the Android root
 * use.
 */

#pragma once

#define	PR_GET_DUMPABLE			3
#define	PR_SET_DUMPABLE			4
#define	PR_SET_NAME			15
#define	PR_GET_NAME			16

#define	PR_SET_NO_NEW_PRIVS		38
#define	PR_GET_NO_NEW_PRIVS		39

#define	PR_CAP_AMBIENT			47
#define	PR_CAP_AMBIENT_IS_SET		1
#define	PR_CAP_AMBIENT_RAISE		2
#define	PR_CAP_AMBIENT_LOWER		3
#define	PR_CAP_AMBIENT_CLEAR_ALL	4

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
