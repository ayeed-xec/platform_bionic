/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#include <sys/cdefs.h>
#include <sys/types.h>

/*
 * The AF_*, PF_*, SOCK_*, SO_*, MSG_* and SCM_* values, struct sockaddr,
 * struct msghdr, struct cmsghdr and the CMSG_* macros come from this header.
 * They have the FreeBSD values and layouts, because the FreeBSD kernel
 * reads them. The header also gives struct iovec.
 */
#include <bits/freebsd/socket.h>

#include <bits/sockaddr_storage.h>
#include <bits/sa_family_t.h>

__BEGIN_DECLS

struct timespec;

/*
 * Only libc reads this structure: the kernel has no sendmmsg() or
 * recvmmsg() system call. The layout is the one of Android.
 */
struct mmsghdr {
  struct msghdr msg_hdr;
  unsigned int msg_len;
};

/*
 * CMSG_NXTHDR() does not call this function any more. The function stays
 * in libc for source compatibility.
 */
struct cmsghdr* _Nullable __cmsg_nxthdr(struct msghdr* _Nonnull __msg, struct cmsghdr* _Nonnull __cmsg);

int accept(int __fd, struct sockaddr* _Nullable __addr, socklen_t* _Nullable __addr_length);
int accept4(int __fd, struct sockaddr* _Nullable __addr, socklen_t* _Nullable __addr_length, int __flags);
int bind(int __fd, const struct sockaddr* _Nonnull __addr, socklen_t __addr_length);
int connect(int __fd, const struct sockaddr* _Nonnull __addr, socklen_t __addr_length);
int getpeername(int __fd, struct sockaddr* _Nonnull __addr, socklen_t* _Nonnull __addr_length);
int getsockname(int __fd, struct sockaddr* _Nonnull __addr, socklen_t* _Nonnull __addr_length);
int getsockopt(int __fd, int __level, int __option, void* _Nullable __value, socklen_t* _Nonnull __value_length);
int listen(int __fd, int __backlog);
int recvmmsg(int __fd, struct mmsghdr* _Nonnull __msgs, unsigned int __msg_count, int __flags, const struct timespec* _Nullable __timeout);
ssize_t recvmsg(int __fd, struct msghdr* _Nonnull __msg, int __flags);
int sendmmsg(int __fd, const struct mmsghdr* _Nonnull __msgs, unsigned int __msg_count, int __flags);
ssize_t sendmsg(int __fd, const struct msghdr* _Nonnull __msg, int __flags);
int setsockopt(int __fd, int __level, int __option, const void* _Nullable __value, socklen_t __value_length);
int shutdown(int __fd, int __how);
int socket(int __af, int __type, int __protocol);
int socketpair(int __af, int __type, int __protocol, int __fds[_Nonnull 2]);

ssize_t recv(int __fd, void* _Nullable __buf, size_t __n, int __flags);
ssize_t send(int __fd, const void* _Nonnull __buf, size_t __n, int __flags);

ssize_t sendto(int __fd, const void* _Nonnull __buf, size_t __n, int __flags, const struct sockaddr* _Nullable __dst_addr, socklen_t __dst_addr_length);
ssize_t recvfrom(int __fd, void* _Nullable __buf, size_t __n, int __flags, struct sockaddr* _Nullable __src_addr, socklen_t* _Nullable __src_addr_length);

#if defined(__BIONIC_INCLUDE_FORTIFY_HEADERS)
#include <bits/fortify/socket.h>
#endif

__END_DECLS

#endif
