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
 * @file sys/gpio.h
 * @brief FreeBSD's gpioc(4) device, which has the pins of one GPIO
 * controller.  It is a FreeBSD-only API, not part of the NDK.  abi_check.c
 * compares it with the FreeBSD header.
 */

#include <sys/types.h>

#include <bits/freebsd/ioccom.h>
#include <stdint.h>

#define	GPIOMAXNAME		64

#define	GPIO_PIN_INPUT		0x00000001	/* input direction */
#define	GPIO_PIN_OUTPUT		0x00000002	/* output direction */
#define	GPIO_PIN_INVIN		0x00000080	/* invert input */
#define	GPIO_PIN_INVOUT		0x00000100	/* invert output */

struct gpio_pin {
	uint32_t	gp_pin;
	char		gp_name[GPIOMAXNAME];
	uint32_t	gp_caps;
	uint32_t	gp_flags;
};

struct gpio_req {
	uint32_t	gp_pin;
	uint32_t	gp_value;
};

#define	GPIOMAXPIN	_IOR('G', 0, int)
#define	GPIOGETCONFIG	_IOWR('G', 1, struct gpio_pin)
#define	GPIOSETCONFIG	_IOW('G', 2, struct gpio_pin)
#define	GPIOGET		_IOWR('G', 3, struct gpio_req)
#define	GPIOSET		_IOW('G', 4, struct gpio_req)
