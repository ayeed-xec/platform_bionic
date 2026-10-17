/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * No GWP-ASan yet; it comes with scudo.  Until then GWP-ASan is always
 * off and android_mallopt(M_INITIALIZE_GWP_ASAN) fails.
 */

#include "bionic/gwp_asan_wrappers.h"

bool
EnableGwpAsan(const android_mallopt_gwp_asan_options_t &)
{
	return (false);
}

bool
MaybeInitGwpAsanFromLibc(libc_globals *)
{
	return (false);
}

bool
DispatchIsGwpAsan(const MallocDispatch *)
{
	return (false);
}
