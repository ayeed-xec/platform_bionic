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
 * Heap sort stand-in for qsort(3).  Android takes qsort() from
 * external/llvm-libc, which this build does not have yet.  Heap sort needs
 * no extra memory and stays O(n log n) in the worst case.  Like qsort(),
 * it is not stable.
 */

#include <stdlib.h>

typedef int	(*compar_t)(const void *, const void *);

static void	swap_elements(char *, char *, size_t);
static void	sift_down(char *, size_t, size_t, size_t, compar_t);

static void
swap_elements(char *a, char *b, size_t size)
{
	char tmp;

	while (size-- > 0) {
		tmp = *a;
		*a++ = *b;
		*b++ = tmp;
	}
}

/* Sift the element at root down until its subtree is a heap again. */
static void
sift_down(char *base, size_t root, size_t n, size_t size, compar_t compar)
{
	size_t child;

	while ((child = 2 * root + 1) < n) {
		if (child + 1 < n &&
		    compar(base + child * size, base + (child + 1) * size) < 0)
			child++;
		if (compar(base + root * size, base + child * size) >= 0)
			return;
		swap_elements(base + root * size, base + child * size, size);
		root = child;
	}
}

void
qsort(void *array, size_t n, size_t size, compar_t compar)
{
	char *base;
	size_t i;

	if (n < 2 || size == 0)
		return;
	base = array;
	for (i = n / 2; i > 0; i--)
		sift_down(base, i - 1, n, size, compar);
	for (i = n - 1; i > 0; i--) {
		swap_elements(base, base + i * size, size);
		sift_down(base, 0, i, size, compar);
	}
}
