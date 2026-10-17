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

/*
 * sendmmsg(2) for bionic-freebsd.  FreeBSD has no sendmmsg system call.  As
 * in FreeBSD's libc, this sends the messages one at a time with sendmsg(2).
 * An error after the first message stops the loop, and the count of sent
 * messages is returned; the next call reports the error.  As on Linux, at
 * most UIO_MAXIOV messages are sent.
 */

#include <sys/socket.h>
#include <sys/uio.h>

ssize_t	__sendmsg(int, const struct msghdr *, int);

int
__sendmmsg(int fd, const struct mmsghdr *msgs, unsigned int n, int flags)
{
	struct mmsghdr *m;
	unsigned int i;
	ssize_t len;

	if (n > UIO_MAXIOV)
		n = UIO_MAXIOV;
	/* The kernel writes msg_len on Linux, so bionic's const is not kept. */
	m = (struct mmsghdr *)msgs;
	for (i = 0; i < n; i++) {
		len = __sendmsg(fd, &m[i].msg_hdr, flags);
		if (len == -1)
			return (i == 0 ? -1 : (int)i);
		m[i].msg_len = (unsigned int)len;
	}
	return ((int)n);
}
