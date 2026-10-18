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
 * sched_getparam(2), sched_setparam(2), sched_getscheduler(2) and
 * sched_setscheduler(2) for bionic-freebsd.
 *
 * On Linux these calls take a thread id, and bionic's pthread functions
 * pass one.  The FreeBSD system calls take a process id and act on the
 * first thread of that process, so libc does the work with
 * rtprio_thread(2), which acts on one thread.  The policies and priority
 * ranges are those of the FreeBSD system calls, so they agree with
 * sched_get_priority_min(2) and sched_get_priority_max(2).  The timeshare
 * scheduler computes the priority of a SCHED_OTHER thread itself, so the
 * priority of such a thread reads back as 0, as on Linux and in libthr.
 *
 * FreeBSD thread ids start above PID_MAX.  An id that names no thread can
 * still be a process id, and the FreeBSD system call then handles it.
 *
 * FreeBSD has no SCHED_RESET_ON_FORK, so a policy with that flag fails
 * with EINVAL.
 */

#include <sys/types.h>

#include <errno.h>
#include <sched.h>

/* FreeBSD <sys/rtprio.h> and <sys/priority.h> values. */
#define	RTP_LOOKUP		0
#define	RTP_SET			1
#define	RTP_PRIO_REALTIME	2
#define	RTP_PRIO_NORMAL		3
#define	RTP_PRIO_FIFO		10
#define	RTP_PRIO_MAX		31
#define	PRI_MIN_TIMESHARE	56
#define	PRI_MAX_TIMESHARE	223

struct rtprio {
	u_short	type;
	u_short	prio;
};

int	__rtprio_thread(int, pid_t, struct rtprio *);
int	__sched_getparam(pid_t, struct sched_param *);
int	__sched_getscheduler(pid_t);
int	__sched_setparam(pid_t, const struct sched_param *);
int	__sched_setscheduler(pid_t, int, const struct sched_param *);

static int
rtprio_policy(const struct rtprio *rtp, int *priority)
{
	switch (rtp->type) {
	case RTP_PRIO_FIFO:
		*priority = RTP_PRIO_MAX - rtp->prio;
		return (SCHED_FIFO);
	case RTP_PRIO_REALTIME:
		*priority = RTP_PRIO_MAX - rtp->prio;
		return (SCHED_RR);
	default:
		*priority = 0;
		return (SCHED_OTHER);
	}
}

static int
policy_rtprio(int policy, int priority, struct rtprio *rtp)
{
	switch (policy) {
	case SCHED_FIFO:
	case SCHED_RR:
		if (priority < 0 || priority > RTP_PRIO_MAX)
			break;
		rtp->type = policy == SCHED_FIFO ? RTP_PRIO_FIFO :
		    RTP_PRIO_REALTIME;
		rtp->prio = RTP_PRIO_MAX - priority;
		return (0);
	case SCHED_OTHER:
		if (priority < 0 ||
		    priority > PRI_MAX_TIMESHARE - PRI_MIN_TIMESHARE)
			break;
		rtp->type = RTP_PRIO_NORMAL;
		rtp->prio = 0;
		return (0);
	}
	errno = EINVAL;
	return (-1);
}

int
sched_getscheduler(pid_t id)
{
	struct rtprio rtp;
	int priority;

	if (id < 0) {
		errno = EINVAL;
		return (-1);
	}
	if (__rtprio_thread(RTP_LOOKUP, id, &rtp) != 0)
		return (errno == ESRCH ? __sched_getscheduler(id) : -1);
	return (rtprio_policy(&rtp, &priority));
}

int
sched_getparam(pid_t id, struct sched_param *param)
{
	struct rtprio rtp;

	if (id < 0) {
		errno = EINVAL;
		return (-1);
	}
	if (__rtprio_thread(RTP_LOOKUP, id, &rtp) != 0)
		return (errno == ESRCH ? __sched_getparam(id, param) : -1);
	rtprio_policy(&rtp, &param->sched_priority);
	return (0);
}

int
sched_setscheduler(pid_t id, int policy, const struct sched_param *param)
{
	struct rtprio rtp;

	if (id < 0) {
		errno = EINVAL;
		return (-1);
	}
	if (policy_rtprio(policy, param->sched_priority, &rtp) != 0)
		return (-1);
	if (__rtprio_thread(RTP_SET, id, &rtp) != 0)
		return (errno == ESRCH ?
		    __sched_setscheduler(id, policy, param) : -1);
	return (0);
}

int
sched_setparam(pid_t id, const struct sched_param *param)
{
	struct rtprio rtp;
	int policy, priority;

	if (id < 0) {
		errno = EINVAL;
		return (-1);
	}
	if (__rtprio_thread(RTP_LOOKUP, id, &rtp) != 0)
		return (errno == ESRCH ? __sched_setparam(id, param) : -1);
	policy = rtprio_policy(&rtp, &priority);
	if (policy_rtprio(policy, param->sched_priority, &rtp) != 0)
		return (-1);
	return (__rtprio_thread(RTP_SET, id, &rtp));
}
