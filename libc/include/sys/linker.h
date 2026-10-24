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
 * @file sys/linker.h
 * @brief FreeBSD's kernel linker calls, which load and unload kernel
 * modules.  It is a FreeBSD-only API, not part of the NDK.
 */

#include <sys/cdefs.h>

/** kldunloadf() flag: unload only a module that is not in use. */
#define	LINKER_UNLOAD_NORMAL	0
/** kldunloadf() flag: unload the module even if it is in use. */
#define	LINKER_UNLOAD_FORCE	1

__BEGIN_DECLS

/**
 * kldload(2)
 * loads the kernel module `__file` and the modules that it depends on.  A
 * name without "/" is looked up in the kern.module_path directories.
 *
 * Returns the file id of the module on success, and returns -1 and sets
 * `errno` on failure.
 */
int kldload(const char *_Nonnull __file);

/**
 * kldfind(2)
 * returns the file id of the loaded module `__file`.
 *
 * Returns the file id on success, and returns -1 and sets `errno` on
 * failure.
 */
int kldfind(const char *_Nonnull __file);

/**
 * kldunloadf(2)
 * unloads the module with the file id `__file_id`, with the flag
 * LINKER_UNLOAD_NORMAL or LINKER_UNLOAD_FORCE.
 *
 * Returns 0 on success, and returns -1 and sets `errno` on failure.
 */
int kldunloadf(int __file_id, int __flags);

__END_DECLS
