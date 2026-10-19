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
 * mremap(2) as Android declares it: the new address is an optional
 * argument that only MREMAP_FIXED reads, and a new size that rounds up past
 * PTRDIFF_MAX fails with ENOMEM, so that no "end - start" can overflow.
 */

#include <sys/cdefs.h>
#include <sys/mman.h>

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

void	*__mremap(void *, size_t, size_t, int, void *);

void *
mremap(void *old_address, size_t old_size, size_t new_size, int flags, ...)
{
	va_list ap;
	void *new_address;
	size_t mask, rounded;

	mask = (size_t)getpagesize() - 1;
	rounded = (new_size + mask) & ~mask;
	if (rounded < new_size || rounded > PTRDIFF_MAX) {
		errno = ENOMEM;
		return (MAP_FAILED);
	}
	new_address = NULL;
	if ((flags & MREMAP_FIXED) != 0) {
		va_start(ap, flags);
		new_address = va_arg(ap, void *);
		va_end(ap);
	}
	return (__mremap(old_address, old_size, new_size, flags, new_address));
}
