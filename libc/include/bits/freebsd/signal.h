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
 * Signal numbers, flags and structures of the FreeBSD kernel ABI.
 * The values and layouts are those of FreeBSD sys/sys/signal.h,
 * sys/sys/_sigset.h and sys/sys/_sigaltstack.h.  abi_check.c checks them.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

#define	SIGHUP		1
#define	SIGINT		2
#define	SIGQUIT		3
#define	SIGILL		4
#define	SIGTRAP		5
#define	SIGABRT		6
#define	SIGIOT		SIGABRT
#define	SIGEMT		7
#define	SIGFPE		8
#define	SIGKILL		9
#define	SIGBUS		10
#define	SIGSEGV		11
#define	SIGSYS		12
#define	SIGPIPE		13
#define	SIGALRM		14
#define	SIGTERM		15
#define	SIGURG		16
#define	SIGSTOP		17
#define	SIGTSTP		18
#define	SIGCONT		19
#define	SIGCHLD		20
#define	SIGTTIN		21
#define	SIGTTOU		22
#define	SIGIO		23
#define	SIGXCPU		24
#define	SIGXFSZ		25
#define	SIGVTALRM	26
#define	SIGPROF		27
#define	SIGWINCH	28
#define	SIGINFO		29
#define	SIGUSR1		30
#define	SIGUSR2		31
#define	SIGTHR		32
#define	SIGLIBRT	33

/* The kernel's real-time signal range.  <signal.h> gives the libc range. */
#define	__SIGRTMIN	65
#define	__SIGRTMAX	126

#define	SA_ONSTACK	0x0001
#define	SA_RESTART	0x0002
#define	SA_RESETHAND	0x0004
#define	SA_NOCLDSTOP	0x0008
#define	SA_NODEFER	0x0010
#define	SA_NOCLDWAIT	0x0020
#define	SA_SIGINFO	0x0040

#define	SS_ONSTACK	0x0001
#define	SS_DISABLE	0x0004
#define	MINSIGSTKSZ	(1024 * 4)
#define	SIGSTKSZ	(MINSIGSTKSZ + 32768)

#define	SIG_BLOCK	1
#define	SIG_UNBLOCK	2
#define	SIG_SETMASK	3

/* For sigqueue(2): the pid argument is a thread ID. */
#define	__SIGQUEUE_TID	0x80000000

typedef void (*__sighandler_t)(int);

#define	SIG_DFL		((__sighandler_t)0)
#define	SIG_IGN		((__sighandler_t)1)
#define	SIG_ERR		((__sighandler_t)-1)

typedef struct __sigset {
	__uint32_t	__bits[4];
} sigset_t;

typedef struct sigaltstack {
	void		*ss_sp;
	size_t		ss_size;
	int		ss_flags;
} stack_t;

union sigval {
	int		sival_int;
	void		*sival_ptr;
};

/* Linux name for union sigval; Android code uses it. */
typedef union sigval	sigval_t;

typedef struct siginfo {
	int		si_signo;
	int		si_errno;
	int		si_code;
	pid_t		si_pid;
	uid_t		si_uid;
	int		si_status;
	void		*si_addr;
	union sigval	si_value;
	union {
		struct {
			int	_trapno;
		} _fault;
		struct {
			int	_timerid;
			int	_overrun;
		} _timer;
		struct {
			int	_mqd;
		} _mesgq;
		struct {
			long	_band;
		} _poll;
		struct {
			int	_syscall;
		} _capsicum;
		struct {
			long	__spare1__;
			int	__spare2__[7];
		} __spare__;
	} _reason;
} siginfo_t;

#define	si_trapno	_reason._fault._trapno
#define	si_timerid	_reason._timer._timerid
#define	si_overrun	_reason._timer._overrun
#define	si_mqd		_reason._mesgq._mqd
#define	si_band		_reason._poll._band
#define	si_syscall	_reason._capsicum._syscall

#define	SI_NOINFO	0
#define	SI_USER		0x10001
#define	SI_QUEUE	0x10002
#define	SI_TIMER	0x10003
#define	SI_ASYNCIO	0x10004
#define	SI_MESGQ	0x10005
#define	SI_KERNEL	0x10006
#define	SI_LWP		0x10007
/* A signal from thr_kill(2) or thr_kill2(2): Linux names it SI_TKILL. */
#define	SI_TKILL	SI_LWP

#define	ILL_ILLOPC	1
#define	ILL_ILLOPN	2
#define	ILL_ILLADR	3
#define	ILL_ILLTRP	4
#define	ILL_PRVOPC	5
#define	ILL_PRVREG	6
#define	ILL_COPROC	7
#define	ILL_BADSTK	8

#define	BUS_ADRALN	1
#define	BUS_ADRERR	2
#define	BUS_OBJERR	3
#define	BUS_OOMERR	100

#define	SEGV_MAPERR	1
#define	SEGV_ACCERR	2

#define	FPE_INTOVF	1
#define	FPE_INTDIV	2
#define	FPE_FLTDIV	3
#define	FPE_FLTOVF	4
#define	FPE_FLTUND	5
#define	FPE_FLTRES	6
#define	FPE_FLTINV	7
#define	FPE_FLTSUB	8
#define	FPE_FLTIDO	9

#define	TRAP_BRKPT	1
#define	TRAP_TRACE	2
#define	TRAP_DTRACE	3
#define	TRAP_CAP	4

#define	CLD_EXITED	1
#define	CLD_KILLED	2
#define	CLD_DUMPED	3
#define	CLD_TRAPPED	4
#define	CLD_STOPPED	5
#define	CLD_CONTINUED	6

#define	POLL_IN		1
#define	POLL_OUT	2
#define	POLL_MSG	3
#define	POLL_ERR	4
#define	POLL_PRI	5
#define	POLL_HUP	6

struct pthread_attr;

typedef struct sigevent {
	int		sigev_notify;
	int		sigev_signo;
	union sigval	sigev_value;
	union {
		__int32_t	_threadid;
		struct {
			void	(*_function)(union sigval);
			struct pthread_attr **_attribute;
		} _sigev_thread;
		unsigned short	_kevent_flags;
		long		__spare__[8];
	} _sigev_un;
} sigevent_t;

#define	sigev_notify_kqueue		sigev_signo
#define	sigev_notify_kevent_flags	_sigev_un._kevent_flags
#define	sigev_notify_thread_id		_sigev_un._threadid
#define	sigev_notify_function		_sigev_un._sigev_thread._function
#define	sigev_notify_attributes		_sigev_un._sigev_thread._attribute

#define	SIGEV_NONE	0
#define	SIGEV_SIGNAL	1
#define	SIGEV_THREAD	2
#define	SIGEV_KEVENT	3
#define	SIGEV_THREAD_ID	4
