/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// The tier-0 allocator of bionic-freebsd (M1): the malloc API on top of
// BionicAllocator, the allocator of linker64. One lock serializes all calls.
// BionicAllocator aborts the process if mmap(2) fails, and it aligns to one
// page at most. The M1 allocator decision (jemalloc or scudo) replaces this
// file and malloc_common.cpp comes back.

#include <errno.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "platform/bionic/page.h"
#include "private/bionic_allocator.h"
#include "private/bionic_globals.h"
#include "private/bionic_lock.h"

static BionicAllocator g_allocator;
static Lock g_lock;

static bool is_power_of_2(size_t n) {
  return n != 0 && (n & (n - 1)) == 0;
}

extern "C" void* malloc(size_t size) {
  LockGuard guard(g_lock);
  return g_allocator.alloc(size);
}

extern "C" void* calloc(size_t count, size_t size) {
  size_t total;
  if (__builtin_mul_overflow(count, size, &total)) {
    errno = ENOMEM;
    return nullptr;
  }
  void* result = malloc(total);
  if (result != nullptr) memset(result, 0, total);
  return result;
}

extern "C" void* realloc(void* ptr, size_t size) {
  LockGuard guard(g_lock);
  return g_allocator.realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, size_t count, size_t size) {
  size_t total;
  if (__builtin_mul_overflow(count, size, &total)) {
    errno = ENOMEM;
    return nullptr;
  }
  return realloc(ptr, total);
}

extern "C" void free(void* ptr) {
  LockGuard guard(g_lock);
  g_allocator.free(ptr);
}

// As bionic's other allocators do, memalign() rounds the alignment up to a
// power of 2.
extern "C" void* memalign(size_t alignment, size_t size) {
  if (alignment > page_size()) {
    errno = EINVAL;
    return nullptr;
  }
  LockGuard guard(g_lock);
  return g_allocator.memalign(alignment, size);
}

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
  if (!is_power_of_2(alignment)) {
    errno = EINVAL;
    return nullptr;
  }
  return memalign(alignment, size);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (alignment < sizeof(void*) || !is_power_of_2(alignment)) return EINVAL;
  void* result = memalign(alignment, size);
  if (result == nullptr) return errno;
  *memptr = result;
  return 0;
}

extern "C" size_t malloc_usable_size(const void* ptr) {
  if (ptr == nullptr) return 0;
  LockGuard guard(g_lock);
  return g_allocator.get_chunk_size(const_cast<void*>(ptr));
}

__LIBC_HIDDEN__ void __libc_init_malloc(libc_globals*) {}
