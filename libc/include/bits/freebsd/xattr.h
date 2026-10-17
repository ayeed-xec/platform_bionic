/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * xattr flags and names.  FreeBSD has extattr(2), not the xattr system
 * calls: libc emulates the xattr calls, so the flags keep the Android
 * values (ADR 0002, decision 1.3).
 */

#pragma once

#define	XATTR_CREATE		0x1
#define	XATTR_REPLACE		0x2

#define	XATTR_NAME_SELINUX	"security.selinux"
