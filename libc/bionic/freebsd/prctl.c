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
 * prctl(2) for bionic-freebsd.  FreeBSD has no prctl(2), so libc emulates
 * each option on native system calls.  The other options fail with EINVAL,
 * as on a Linux kernel that does not have the option.  The startup code
 * uses PR_SET_VMA (names for memory maps), and it ignores the failure.
 *
 * PR_SET_NAME	thr_set_name(2) on the calling thread.  The name is cut to
 *		15 bytes, as on Linux.
 * PR_GET_DUMPABLE, PR_SET_DUMPABLE
 *		procctl(2) PROC_TRACE_STATUS and PROC_TRACE_CTL: on FreeBSD,
 *		this switch controls both core dumps and ptrace(2).  A
 *		set-user-ID process is never dumpable.
 */

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

/* FreeBSD <sys/wait.h> idtype_t and <sys/procctl.h> values. */
#define	FREEBSD_P_PID			0
#define	PROC_TRACE_CTL			7
#define	PROC_TRACE_STATUS		8
#define	PROC_TRACE_CTL_ENABLE		1
#define	PROC_TRACE_CTL_DISABLE		2

/* Linux keeps 16 bytes of a thread name, with the terminating NUL. */
#define	TASK_COMM_LEN			16

int	__procctl(int, id_t, int, void *);
int	__thr_self(long *);
int	__thr_set_name(long, const char *);

static int
set_name(const char *name)
{
	char buf[TASK_COMM_LEN];
	long tid;

	if (name == NULL) {
		errno = EFAULT;
		return (-1);
	}
	strlcpy(buf, name, sizeof(buf));
	if (__thr_self(&tid) != 0)
		return (-1);
	return (__thr_set_name(tid, buf));
}

static int
get_dumpable(void)
{
	int status;

	if (issetugid())
		return (0);
	if (__procctl(FREEBSD_P_PID, getpid(), PROC_TRACE_STATUS,
	    &status) != 0)
		return (-1);
	return (status == -1 ? 0 : 1);
}

static int
set_dumpable(unsigned long arg)
{
	int ctl;

	switch (arg) {
	case 0:
		ctl = PROC_TRACE_CTL_DISABLE;
		break;
	case 1:
		ctl = PROC_TRACE_CTL_ENABLE;
		break;
	default:
		errno = EINVAL;
		return (-1);
	}
	return (__procctl(FREEBSD_P_PID, getpid(), PROC_TRACE_CTL, &ctl));
}

int
prctl(int op, ...)
{
	va_list ap;
	unsigned long arg;

	va_start(ap, op);
	arg = va_arg(ap, unsigned long);
	va_end(ap);

	switch (op) {
	case PR_SET_NAME:
		return (set_name((const char *)arg));
	case PR_GET_DUMPABLE:
		return (get_dumpable());
	case PR_SET_DUMPABLE:
		return (set_dumpable(arg));
	default:
		errno = EINVAL;
		return (-1);
	}
}
