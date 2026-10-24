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

#pragma once

/**
 * @file net/bpf.h
 * @brief FreeBSD's bpf(4) device, which reads and writes the frames of a
 * network interface, and the filter programs that it runs.  It is a
 * FreeBSD-only API, not part of the NDK.  abi_check.c compares it with the
 * FreeBSD header.
 */

#include <sys/types.h>
#include <sys/time.h>

#include <bits/freebsd/ioccom.h>
#include <net/if.h>
#include <stdint.h>

struct bpf_insn {
	unsigned short	code;
	unsigned char	jt;
	unsigned char	jf;
	uint32_t	k;
};

struct bpf_program {
	unsigned int	bf_len;
	struct bpf_insn	*bf_insns;
};

/** The header of each frame that read(2) gives. */
struct bpf_hdr {
	struct timeval	bh_tstamp;
	uint32_t	bh_caplen;
	uint32_t	bh_datalen;
	unsigned short	bh_hdrlen;
};

#define	BPF_ALIGNMENT	sizeof(long)
#define	BPF_WORDALIGN(x) (((x) + (BPF_ALIGNMENT - 1)) & ~(BPF_ALIGNMENT - 1))

#define	BIOCSBLEN	_IOWR('B', 102, unsigned int)
#define	BIOCSETF	_IOW('B', 103, struct bpf_program)
#define	BIOCSETIF	_IOW('B', 108, struct ifreq)
#define	BIOCIMMEDIATE	_IOW('B', 112, unsigned int)

/* Instruction classes. */
#define	BPF_LD		0x00
#define	BPF_LDX		0x01
#define	BPF_JMP		0x05
#define	BPF_RET		0x06
/* Load sizes. */
#define	BPF_H		0x08
#define	BPF_B		0x10
/* Load modes. */
#define	BPF_ABS		0x20
#define	BPF_IND		0x40
#define	BPF_MSH		0xa0
/* Jump tests. */
#define	BPF_JEQ		0x10
#define	BPF_JSET	0x40
/* Operand sources. */
#define	BPF_K		0x00

#define	BPF_STMT(code, k)		{ (unsigned short)(code), 0, 0, k }
#define	BPF_JUMP(code, k, jt, jf)	{ (unsigned short)(code), jt, jf, k }
