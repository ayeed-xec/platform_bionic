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
 * recvmsg(2) for bionic-freebsd: the flags that it returns, and the
 * credentials of SO_PASSCRED.
 *
 * FreeBSD hands the flags argument to the protocol in msg_flags and returns
 * that field, so a stream socket reports MSG_PEEK, MSG_DONTWAIT, MSG_TRUNC
 * and the other input flags as if the receive had found them.  POSIX and
 * Linux return only what the receive found: MSG_EOR, MSG_OOB, MSG_TRUNC and
 * MSG_CTRUNC.  MSG_CTRUNC means nothing as an input flag, so it does not go
 * to the kernel.  With MSG_TRUNC a datagram socket returns the full length
 * of a longer datagram, so the message was cut exactly when that length is
 * larger than the buffers.
 *
 * A socket with SO_PASSCRED (LOCAL_CREDS_PERSISTENT, see sockopt.c) gets
 * the credentials of the sender as SCM_CREDS2, a struct sockcred2 with up
 * to CMGROUP_MAX groups.  The caller expects SCM_CREDENTIALS, a smaller
 * struct ucred, and sizes its control buffer for that.  So the kernel gets
 * a buffer that is larger by the difference, and the messages go back to
 * the caller with each SCM_CREDS2 as a SCM_CREDENTIALS.  A message that does
 * not fit sets MSG_CTRUNC, and the descriptors of a SCM_RIGHTS message that
 * do not fit are closed.  A control buffer that is larger than the stack
 * buffer goes to the kernel as it is and is converted in place: a converted
 * message is never longer than the original.
 *
 * recvmsg() and recvmmsg() both call this.
 */

#include <sys/socket.h>
#include <sys/uio.h>

#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "local_creds.h"

#define	RECV_RESULT_FLAGS \
	(MSG_OOB | MSG_EOR | MSG_TRUNC | MSG_CTRUNC | MSG_NOTIFICATION)

/* How much longer SCM_CREDS2 can be than the SCM_CREDENTIALS message. */
#define	CREDS_ROOM \
	(CMSG_SPACE(__FREEBSD_SOCKCRED2SIZE(__FREEBSD_CMGROUP_MAX)) - \
	    CMSG_SPACE(sizeof(struct ucred)))
#define	CONTROL_STACK	1024

ssize_t	__recvmsg(int, struct msghdr *, int);
ssize_t	__sys_recvmsg(int, struct msghdr *, int);

/* Close the descriptors from index first of each SCM_RIGHTS message. */
static void
close_rights(const char *p, const char *end, size_t first)
{
	struct cmsghdr h;
	size_t i, n;
	int fd;

	while (p + sizeof(h) <= end) {
		memcpy(&h, p, sizeof(h));
		if (h.cmsg_len < CMSG_LEN(0) || h.cmsg_len > (size_t)(end - p))
			break;
		if (h.cmsg_level == SOL_SOCKET && h.cmsg_type == SCM_RIGHTS) {
			n = (h.cmsg_len - CMSG_LEN(0)) / sizeof(int);
			for (i = first; i < n; i++) {
				memcpy(&fd, p + CMSG_LEN(i * sizeof(int)),
				    sizeof(fd));
				close(fd);
			}
		}
		first = 0;
		p += CMSG_ALIGN(h.cmsg_len);
	}
}

/*
 * Copy the control messages in src, len bytes long, to dst, which has room
 * for cap bytes, with each SCM_CREDS2 as a SCM_CREDENTIALS.  dst can be
 * src.  Return the length of the messages in dst.
 */
static socklen_t
control_to_linux(char *dst, socklen_t cap, const char *src, socklen_t len,
    int *flags)
{
	struct __freebsd_sockcred2 sc;
	struct cmsghdr h;
	struct ucred uc;
	const char *end, *p;
	size_t fit, n, out;

	out = 0;
	p = src;
	end = src + len;
	while (p + sizeof(h) <= end) {
		memcpy(&h, p, sizeof(h));
		if (h.cmsg_len < sizeof(h) || h.cmsg_len > (size_t)(end - p))
			break;
		if (h.cmsg_level == SOL_SOCKET &&
		    h.cmsg_type == __FREEBSD_SCM_CREDS2 && h.cmsg_len >=
		    CMSG_LEN(offsetof(struct __freebsd_sockcred2, sc_groups))) {
			n = CMSG_LEN(sizeof(uc));
			if (out + n > cap)
				goto truncated;
			memcpy(&sc, p + CMSG_LEN(0),
			    offsetof(struct __freebsd_sockcred2, sc_groups));
			uc.pid = sc.sc_pid;
			uc.uid = sc.sc_uid;
			uc.gid = sc.sc_gid;
			p += CMSG_ALIGN(h.cmsg_len);
			h.cmsg_len = n;
			h.cmsg_type = SCM_CREDENTIALS;
			memset(dst + out, 0, CMSG_LEN(0));
			memcpy(dst + out, &h, sizeof(h));
			memcpy(dst + out + CMSG_LEN(0), &uc, sizeof(uc));
		} else {
			n = h.cmsg_len;
			if (out + n > cap)
				goto truncated;
			memmove(dst + out, p, n);
			p += CMSG_ALIGN(n);
		}
		out += CMSG_ALIGN(n);
		if (out > cap)
			out = cap;
	}
	return (out);

truncated:
	*flags |= MSG_CTRUNC;
	fit = 0;
	if (h.cmsg_level == SOL_SOCKET && h.cmsg_type == SCM_RIGHTS &&
	    out + CMSG_LEN(sizeof(int)) <= cap)
		fit = (cap - out - CMSG_LEN(0)) / sizeof(int);
	/* Close first: the copy can write over the source in place. */
	close_rights(p, end, fit);
	if (fit > 0) {
		n = CMSG_LEN(fit * sizeof(int));
		memmove(dst + out, p, n);
		h.cmsg_len = n;
		memcpy(dst + out, &h, sizeof(h));
		out += n;
	}
	return (out);
}

ssize_t
__recvmsg(int fd, struct msghdr *msg, int flags)
{
	union {
		struct cmsghdr	h;
		char		buf[CONTROL_STACK];
	} control;
	struct msghdr kmsg, *m;
	socklen_t cap;
	size_t len;
	ssize_t n;
	int i;

	m = msg;
	cap = msg->msg_controllen;
	if (msg->msg_control != NULL && cap > 0 &&
	    cap <= sizeof(control) - CREDS_ROOM) {
		kmsg = *msg;
		kmsg.msg_control = control.buf;
		kmsg.msg_controllen = cap + CREDS_ROOM;
		m = &kmsg;
	}
	n = __sys_recvmsg(fd, m, flags & ~MSG_CTRUNC);
	if (n == -1)
		return (-1);
	if (m != msg) {
		msg->msg_namelen = kmsg.msg_namelen;
		msg->msg_flags = kmsg.msg_flags;
	}
	if (msg->msg_control != NULL)
		msg->msg_controllen = control_to_linux(msg->msg_control, cap,
		    m->msg_control, m->msg_controllen, &msg->msg_flags);
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
