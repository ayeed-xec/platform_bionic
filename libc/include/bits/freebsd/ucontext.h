/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Machine context of the FreeBSD kernel ABI.  The layout is that of
 * FreeBSD sys/arm64/include/ucontext.h.  abi_check.c checks it.
 */

#pragma once

#include <sys/cdefs.h>
#include <stdint.h>

#if defined(__aarch64__)

struct gpregs {
	int64_t		gp_x[30];
	int64_t		gp_lr;
	int64_t		gp_sp;
	int64_t		gp_elr;
	uint64_t	gp_spsr;
};

struct fpregs {
	__uint128_t	fp_q[32];
	uint32_t	fp_sr;
	uint32_t	fp_cr;
	int		fp_flags;
	int		fp_pad;
};

typedef struct __mcontext {
	struct gpregs	mc_gpregs;
	struct fpregs	mc_fpregs;
	int		mc_flags;
#define	_MC_FP_VALID	0x1
	int		mc_pad;
	uint64_t	mc_ptr;
	uint64_t	mc_spare[7];
} mcontext_t;

#else
#error "bionic-freebsd: no machine context for this architecture yet"
#endif
