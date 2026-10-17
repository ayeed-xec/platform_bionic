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

// Tier-0 stand-in for bionic's ICU4X layer (bionic/icu4x.rs). That layer is
// Rust, and bionic-freebsd does not build Rust yet. wctype.cpp and wcwidth.cpp
// handle ASCII themselves and call these functions only for other code
// points. Here those code points get the answers of the C locale: no class,
// no case mapping, unassigned and neutral width. bionic gave the same answers
// when it could not load ICU. Remove this file when icu4x.rs is built.

#include "private/icu4x.h"

uint8_t __icu4x_bionic_general_category(uint32_t) {
  return 0;  // Cn, unassigned.
}

uint8_t __icu4x_bionic_east_asian_width(uint32_t) {
  return U_EA_NEUTRAL;
}

uint8_t __icu4x_bionic_hangul_syllable_type(uint32_t) {
  return U_HST_NOT_APPLICABLE;
}

bool __icu4x_bionic_is_alphabetic(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_default_ignorable_code_point(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_lowercase(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_alnum(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_blank(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_graph(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_print(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_xdigit(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_white_space(uint32_t) {
  return false;
}

bool __icu4x_bionic_is_uppercase(uint32_t) {
  return false;
}

uint32_t __icu4x_bionic_to_upper(uint32_t ch) {
  return ch;
}

uint32_t __icu4x_bionic_to_lower(uint32_t ch) {
  return ch;
}
