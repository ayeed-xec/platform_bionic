/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// elf_aux_info(3) of FreeBSD, for code that is built for the FreeBSD target,
// such as the compiler-rt builtins (out-of-line atomics). It reads the same
// auxiliary vector as getauxval(3). The items and the buffer rules are those
// of the FreeBSD auxv(3) manual page.

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/auxv.h>

#include "private/bionic_auxv.h"

// Copies an item whose buffer must have exactly the size of the value.
template <typename T>
static int copy_value(int aux, void* buf, int buflen) {
  if (buflen != static_cast<int>(sizeof(T))) return EINVAL;
  bool exists;
  T value = static_cast<T>(__bionic_getauxval(aux, &exists));
  if (!exists) return ENOENT;
  memcpy(buf, &value, sizeof(value));
  return 0;
}

// Copies as much of a byte array as fits, and zeroes the rest of the buffer.
static int copy_array(int aux, int aux_len, void* buf, int buflen) {
  bool exists;
  const void* src = reinterpret_cast<const void*>(__bionic_getauxval(aux, &exists));
  if (!exists || src == nullptr) return ENOENT;
  size_t len = __bionic_getauxval(aux_len, &exists);
  if (!exists) return ENOENT;
  size_t n = len < static_cast<size_t>(buflen) ? len : buflen;
  memcpy(buf, src, n);
  memset(static_cast<char*>(buf) + n, 0, buflen - n);
  return 0;
}

extern "C" int elf_aux_info(int aux, void* buf, int buflen) {
  if (buflen < 0) return EINVAL;

  switch (aux) {
    case AT_HWCAP:
    case AT_HWCAP2:
    case AT_HWCAP3:
    case AT_HWCAP4:
    case AT_TIMEKEEP:
    case AT_USRSTACKBASE:
    case AT_USRSTACKLIM:
      return copy_value<unsigned long>(aux, buf, buflen);
    case AT_NCPUS:
    case AT_OSRELDATE:
    case AT_PAGESZ:
      return copy_value<int>(aux, buf, buflen);
    case AT_CANARY:
      return copy_array(AT_CANARY, AT_CANARYLEN, buf, buflen);
    case AT_PAGESIZES: {
      // Only whole elements of the page size array.
      int whole = buflen - buflen % static_cast<int>(sizeof(unsigned long));
      return copy_array(AT_PAGESIZES, AT_PAGESIZESLEN, buf, whole);
    }
    case AT_EXECPATH: {
      bool exists;
      const char* path = reinterpret_cast<const char*>(__bionic_getauxval(AT_EXECPATH, &exists));
      if (!exists || path == nullptr) return ENOENT;
      size_t len = strlen(path) + 1;
      if (len > static_cast<size_t>(buflen)) return EINVAL;
      memcpy(buf, path, len);
      return 0;
    }
  }
  return EINVAL;
}
