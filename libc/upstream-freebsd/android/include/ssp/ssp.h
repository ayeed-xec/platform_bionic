/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * FreeBSD's <ssp/ssp.h> declares its own fortified wrappers.  bionic has its
 * own fortify headers, so FreeBSD sources get nothing here; freebsd-compat.h
 * defines __ssp_real().
 */

#pragma once
