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
 * The flags that recvmsg(2) returns, for bionic-freebsd.  FreeBSD hands the
 * flags argument to the protocol in msg_flags and returns that field, so a
 * stream socket reports MSG_PEEK, MSG_DONTWAIT, MSG_TRUNC and the other
 * input flags as if the receive had found them.  POSIX and Linux return
 * only what the receive found: MSG_EOR, MSG_OOB, MSG_TRUNC and MSG_CTRUNC.
 *
 * MSG_CTRUNC means nothing as an input flag, so it does not go to the
 * kernel.  With MSG_TRUNC a datagram socket returns the full length of a
 * longer datagram, so the message was cut exactly when that length is
 * larger than the buffers.  recvmsg() and recvmmsg() both call this.
 */

#include <sys/socket.h>
#include <sys/uio.h>

#define	RECV_RESULT_FLAGS \
	(MSG_OOB | MSG_EOR | MSG_TRUNC | MSG_CTRUNC | MSG_NOTIFICATION)

ssize_t	__recvmsg(int, struct msghdr *, int);
ssize_t	__sys_recvmsg(int, struct msghdr *, int);

ssize_t
__recvmsg(int fd, struct msghdr *msg, int flags)
{
	size_t len;
	ssize_t n;
	int i;

	n = __sys_recvmsg(fd, msg, flags & ~MSG_CTRUNC);
	if (n == -1)
		return (-1);
	msg->msg_flags &= RECV_RESULT_FLAGS;
	if ((flags & MSG_TRUNC) != 0) {
		len = 0;
		for (i = 0; i < msg->msg_iovlen; i++)
			len += msg->msg_iov[i].iov_len;
		if ((size_t)n > len)
			msg->msg_flags |= MSG_TRUNC;
		else
			msg->msg_flags &= ~MSG_TRUNC;
	}
	return (n);
}
