/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
