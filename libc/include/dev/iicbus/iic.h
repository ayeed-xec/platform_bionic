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
 * @file dev/iicbus/iic.h
 * @brief FreeBSD's iic(4) device, which makes I2C transfers on one bus.  It
 * is a FreeBSD-only API, not part of the NDK.  abi_check.c compares it with
 * the FreeBSD header.
 */

#include <sys/types.h>

#include <bits/freebsd/ioccom.h>
#include <stdint.h>

/** One message of a transfer.  The slave address is shifted left by 1. */
struct iic_msg {
	uint16_t	slave;
	uint16_t	flags;
#define	IIC_M_WR	0	/* write */
#define	IIC_M_RD	0x0001	/* read */
#define	IIC_M_NOSTOP	0x0002	/* no stop after the message */
	uint16_t	len;
	uint8_t		*buf;
};

#define	IIC_RDRW_MAX_MSGS	42

struct iic_rdwr_data {
	struct iic_msg	*msgs;
	uint32_t	nmsgs;	/* IIC_RDRW_MAX_MSGS at most */
};

/** Make the messages of a transfer. */
#define	I2CRDWR		_IOW('i', 6, struct iic_rdwr_data)
/** Set the shifted slave address that read(2) and write(2) use. */
#define	I2CSADDR	_IOW('i', 8, uint8_t)
