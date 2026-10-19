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
 * sched_getparam(2), sched_setparam(2), sched_getscheduler(2),
 * sched_setscheduler(2) and sched_rr_get_interval(2) for bionic-freebsd.
 *
 * The FreeBSD system calls take a thread id, as on Linux, and
 * sched_setscheduler(2) takes SCHED_RESET_ON_FORK.  Each function makes
 * one system call.  A negative id gives EINVAL, as on Linux; FreeBSD gives
 * ESRCH.
 *
 * Linux checks the policy and the priority before the privilege, FreeBSD
 * after it, so sched_setscheduler() checks them first.  The priority
 * ranges are those of the FreeBSD system calls, so they agree with
 * sched_get_priority_min(2) and sched_get_priority_max(2).
 *
 * The timeshare scheduler computes the priority of a thread that is not
 * realtime itself, and sched_getparam(2) reports it above the realtime
 * range.  Such a priority reads back as 0, as on Linux and in libthr.
 *
 * SCHED_IDLE is the idle class of rtprio_thread(2) at its lowest priority.
 * FreeBSD lets only root set it, unless security.bsd.unprivileged_idprio
 * is 1.  rtprio_thread(2) takes only a thread id or 0, and no
 * SCHED_RESET_ON_FORK.  The kernel scheduler calls do not know the idle
 * class: sched_getscheduler() reports SCHED_OTHER for such a thread, and
 * sched_setparam() makes it SCHED_OTHER.  FreeBSD has no SCHED_BATCH, so
 * it fails with EINVAL.
 */

#include <sys/types.h>

#include <errno.h>
#include <sched.h>
#include <time.h>

/* FreeBSD <sys/rtprio.h> and <sys/priority.h> values. */
#define	RTP_SET			1
#define	RTP_PRIO_IDLE		4
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
int	__sched_rr_get_interval(pid_t, struct timespec *);
int	__sched_setparam(pid_t, const struct sched_param *);
int	__sched_setscheduler(pid_t, int, const struct sched_param *);

static int
einval(void)
{
	errno = EINVAL;
	return (-1);
}

int
sched_getscheduler(pid_t id)
{
	if (id < 0)
		return (einval());
	return (__sched_getscheduler(id));
}

int
sched_getparam(pid_t id, struct sched_param *param)
{
	if (id < 0)
		return (einval());
	if (__sched_getparam(id, param) != 0)
		return (-1);
	if (param->sched_priority > RTP_PRIO_MAX)
		param->sched_priority = 0;
	return (0);
}

int
sched_setscheduler(pid_t id, int policy, const struct sched_param *param)
{
	struct rtprio rtp;
	int max;

	if (id < 0)
		return (einval());
	switch (policy & ~SCHED_RESET_ON_FORK) {
	case SCHED_FIFO:
	case SCHED_RR:
		max = RTP_PRIO_MAX;
		break;
	case SCHED_OTHER:
		max = PRI_MAX_TIMESHARE - PRI_MIN_TIMESHARE;
		break;
	case SCHED_IDLE:
		if (policy != SCHED_IDLE || param->sched_priority != 0)
			return (einval());
		rtp.type = RTP_PRIO_IDLE;
		rtp.prio = RTP_PRIO_MAX;
		return (__rtprio_thread(RTP_SET, id, &rtp));
	default:
		return (einval());
	}
	if (param->sched_priority < 0 || param->sched_priority > max)
		return (einval());
	return (__sched_setscheduler(id, policy, param));
}

int
sched_setparam(pid_t id, const struct sched_param *param)
{
	if (id < 0)
		return (einval());
	return (__sched_setparam(id, param));
}

int
sched_rr_get_interval(pid_t id, struct timespec *interval)
{
	if (id < 0)
		return (einval());
	return (__sched_rr_get_interval(id, interval));
}
