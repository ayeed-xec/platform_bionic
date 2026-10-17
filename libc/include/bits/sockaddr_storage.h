/*
 * Copyright (C) 2023 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

/**
 * @file bits/sockaddr_storage.h
 * @brief The `sockaddr_storage` struct.
 */

#include <sys/cdefs.h>
#include <stdint.h>

#include <bits/sa_family_t.h>

__BEGIN_DECLS

/*
 * The layout of the FreeBSD kernel ABI (FreeBSD sys/sys/_sockaddr_storage.h):
 * a length byte, the family, padding, an 8-byte aligned member, more padding.
 * The structure is 128 bytes long. abi_check.c checks it.
 */
#define __SS_MAXSIZE 128
#define __SS_ALIGNSIZE sizeof(int64_t)
#define __SS_PAD1SIZE (__SS_ALIGNSIZE - sizeof(unsigned char) - sizeof(sa_family_t))
#define __SS_PAD2SIZE \
  (__SS_MAXSIZE - sizeof(unsigned char) - sizeof(sa_family_t) - __SS_PAD1SIZE - __SS_ALIGNSIZE)

/**
 * [sockaddr_storage](https://man7.org/linux/man-pages/man3/sockaddr_storage.3type.html)
 * is a structure large enough to contain any other `sockaddr_*` type, used to
 * pass socket addresses without needing to know what kind of socket address
 * you're passing.
 */
struct sockaddr_storage {
  unsigned char ss_len;
  sa_family_t ss_family;
  char __ss_pad1[__SS_PAD1SIZE];
  int64_t __ss_align;
  char __ss_pad2[__SS_PAD2SIZE];
};

__END_DECLS
