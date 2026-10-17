/*
 * Copyright (c) 2026 <copyright holder>
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
