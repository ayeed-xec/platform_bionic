<!--
Copyright (c) 2026 Ayeed Shaikh <Ayeed.AI10@race.reva.edu.in>

SPDX-License-Identifier: BSD-2-Clause

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
-->

# The FreeBSD system call table of bionic-freebsd

`syscalls.master` is a copy of `sys/kern/syscalls.master` of the freebsd-src fork,
branch `android-native-15.1`. The kernel that bionic-freebsd runs on is built from that
branch, so its new system calls (for example `epoll_ctl` and `epoll_pwait2`) are in this
file and not in the `syscalls.master` of a stock FreeBSD 15.1.

The build makes the system call stubs (`SYSCALLS.FREEBSD.TXT`) and
`libc/include/bits/freebsd/syscall_numbers.h` from this file. When the fork adds a
system call, copy its `syscalls.master` here in the same change as the rows of
`SYSCALLS.FREEBSD.TXT`.
