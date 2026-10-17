/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
