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

#include "platform/bionic/malloc.h"
#include "bionic/malloc_heapprofd.h"
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

// There is no malloc dispatch (malloc_common_dynamic.cpp): android_mallopt()
// has no options, and there is no heapprofd.
extern "C" bool android_mallopt(int, void*, size_t) {
  errno = ENOTSUP;
  return false;
}

void HandleHeapprofdSignal() {}
