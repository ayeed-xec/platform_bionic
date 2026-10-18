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
 * recvmmsg(2) for bionic-freebsd.  FreeBSD has no recvmmsg system call.  As
 * in FreeBSD's libc, this receives the messages one at a time with
 * recvmsg(2), and MSG_WAITFORONE makes each call after the first one
 * non-blocking.  An error after the first message stops the loop, and the
 * count of received messages is returned; the next call reports the error.
 * As on Linux, at most UIO_MAXIOV messages are received, and the timeout is
 * checked only after each message, so a call can block past it.
 */

#include <sys/socket.h>
#include <sys/uio.h>

#include <errno.h>
#include <time.h>

ssize_t	__recvmsg(int, struct msghdr *, int);

int
__recvmmsg(int fd, struct mmsghdr *msgs, unsigned int n, int flags,
    const struct timespec *timeout)
{
	struct timespec end, now;
	unsigned int i;
	ssize_t len;

	if (timeout != NULL) {
		if (timeout->tv_sec < 0 || timeout->tv_nsec < 0 ||
		    timeout->tv_nsec >= 1000000000) {
			errno = EINVAL;
			return (-1);
		}
		clock_gettime(CLOCK_MONOTONIC, &end);
		end.tv_sec += timeout->tv_sec;
		end.tv_nsec += timeout->tv_nsec;
		if (end.tv_nsec >= 1000000000) {
			end.tv_sec++;
			end.tv_nsec -= 1000000000;
		}
	}
	if (n > UIO_MAXIOV)
		n = UIO_MAXIOV;
	for (i = 0; i < n; i++) {
		len = __recvmsg(fd, &msgs[i].msg_hdr, flags);
		if (len == -1)
			return (i == 0 ? -1 : (int)i);
		msgs[i].msg_len = (unsigned int)len;
		if ((flags & MSG_WAITFORONE) != 0)
			flags |= MSG_DONTWAIT;
		if (timeout != NULL) {
			clock_gettime(CLOCK_MONOTONIC, &now);
			if (now.tv_sec > end.tv_sec ||
			    (now.tv_sec == end.tv_sec &&
			    now.tv_nsec >= end.tv_nsec))
				return ((int)i + 1);
		}
	}
	return ((int)n);
}
