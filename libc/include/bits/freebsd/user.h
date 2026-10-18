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
 * struct kinfo_proc, the record of the kern.proc sysctl(3) names, from
 * FreeBSD sys/sys/user.h, sys/sys/priority.h and sys/sys/proc.h (the ki_stat
 * values).  struct kinfo_vmentry is the record of kern.proc.vmmap, and
 * struct kinfo_file is the record of fcntl(F_KINFO).  abi_check.c checks
 * the layouts.
 *
 * bionic's <sys/user.h> is the Linux core file header, so <sys/sysctl.h>
 * gives these structures.  The kernel addresses keep their FreeBSD types:
 * user space cannot use them.
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>
#include <limits.h>
#include <stdint.h>
#include <bits/freebsd/resource.h>
#include <bits/freebsd/time.h>
#include <bits/signal_types.h>

#define	KINFO_PROC_SIZE	1088

#define	KI_NSPARE_INT	2
#define	KI_NSPARE_LONG	12
#define	KI_NSPARE_PTR	4

#define	WMESGLEN	8
#define	LOCKNAMELEN	8
#define	TDNAMLEN	16
#define	COMMLEN		19
#define	KI_EMULNAMELEN	16
#define	KI_NGROUPS	16
#define	LOGNAMELEN	17
#define	LOGINCLASSLEN	17

/* ki_stat values. */
#define	SIDL	1
#define	SRUN	2
#define	SSLEEP	3
#define	SSTOP	4
#define	SZOMB	5
#define	SWAIT	6
#define	SLOCK	7

struct priority {
	u_char	pri_class;
	u_char	pri_level;
	u_char	pri_native;
	u_char	pri_user;
};

struct kinfo_proc {
	int		ki_structsize;
	int		ki_layout;
	struct pargs	*ki_args;
	struct proc	*ki_paddr;
	struct user	*ki_addr;
	struct vnode	*ki_tracep;
	struct vnode	*ki_textvp;
	struct filedesc	*ki_fd;
	struct vmspace	*ki_vmspace;
	const void	*ki_wchan;
	pid_t		ki_pid;
	pid_t		ki_ppid;
	pid_t		ki_pgid;
	pid_t		ki_tpgid;
	pid_t		ki_sid;
	pid_t		ki_tsid;
	short		ki_jobc;
	short		ki_spare_short1;
	uint32_t	ki_tdev_freebsd11;
	sigset_t	ki_siglist;
	sigset_t	ki_sigmask;
	sigset_t	ki_sigignore;
	sigset_t	ki_sigcatch;
	uid_t		ki_uid;
	uid_t		ki_ruid;
	uid_t		ki_svuid;
	gid_t		ki_rgid;
	gid_t		ki_svgid;
	short		ki_ngroups;
	short		ki_spare_short2;
	gid_t		ki_groups[KI_NGROUPS];
	uint64_t	ki_size;		/* vm_size_t */
	int64_t		ki_rssize;		/* segsz_t */
	int64_t		ki_swrss;
	int64_t		ki_tsize;
	int64_t		ki_dsize;
	int64_t		ki_ssize;
	u_short		ki_xstat;
	u_short		ki_acflag;
	uint32_t	ki_pctcpu;		/* fixpt_t */
	u_int		ki_estcpu;
	u_int		ki_slptime;
	u_int		ki_swtime;
	u_int		ki_cow;
	uint64_t	ki_runtime;
	struct timeval	ki_start;
	struct timeval	ki_childtime;
	long		ki_flag;
	long		ki_kiflag;
	int		ki_traceflag;
	char		ki_stat;
	signed char	ki_nice;
	char		ki_lock;
	char		ki_rqindex;
	u_char		ki_oncpu_old;
	u_char		ki_lastcpu_old;
	char		ki_tdname[TDNAMLEN + 1];
	char		ki_wmesg[WMESGLEN + 1];
	char		ki_login[LOGNAMELEN + 1];
	char		ki_lockname[LOCKNAMELEN + 1];
	char		ki_comm[COMMLEN + 1];
	char		ki_emul[KI_EMULNAMELEN + 1];
	char		ki_loginclass[LOGINCLASSLEN + 1];
	char		ki_moretdname[4];	/* MAXCOMLEN - TDNAMLEN + 1 */
	char		ki_sparestrings[38];
	int		ki_spareints[KI_NSPARE_INT];
	pid_t		ki_reaper;
	pid_t		ki_reapsubtree;
	uint64_t	ki_tdev;
	int		ki_oncpu;
	int		ki_lastcpu;
	int		ki_tracer;
	int		ki_flag2;
	int		ki_fibnum;
	u_int		ki_cr_flags;
	int		ki_jid;
	int		ki_numthreads;
	pid_t		ki_tid;			/* lwpid_t */
	struct priority	ki_pri;
	struct rusage	ki_rusage;
	struct rusage	ki_rusage_ch;
	struct pcb	*ki_pcb;
	void		*ki_kstack;
	void		*ki_udata;
	struct thread	*ki_tdaddr;
	struct pwddesc	*ki_pd;
	void		*ki_uerrmsg;
	void		*ki_spareptrs[KI_NSPARE_PTR];
	long		ki_sparelongs[KI_NSPARE_LONG];
	long		ki_sflag;
	long		ki_tdflags;
};

/* kf_type values. */
#define	KF_TYPE_SHM	8

/* kf_status flags. */
#define	KF_ATTR_VALID	0x0001

/*
 * Of the kf_un union, only kf_file (a vnode or a shm object) is here.  No
 * other member is larger, so the size and the offsets do not change.
 */
struct kinfo_file {
	int		kf_structsize;
	int		kf_type;
	int		kf_fd;
	int		kf_ref_count;
	int		kf_flags;
	int		kf_pad0;
	int64_t		kf_offset;
	union {
		struct {
			int		kf_file_type;
			int		kf_spareint[3];
			uint64_t	kf_spareint64[29];
			uint64_t	kf_file_nlink;
			uint64_t	kf_file_fsid;
			uint64_t	kf_file_rdev;
			uint64_t	kf_file_fileid;
			uint64_t	kf_file_size;
			uint32_t	kf_file_fsid_freebsd11;
			uint32_t	kf_file_rdev_freebsd11;
			uint16_t	kf_file_mode;
			uint16_t	kf_file_pad0;
			uint32_t	kf_file_pad1;
		} kf_file;
	} kf_un;
	uint16_t	kf_status;
	uint16_t	kf_pad1;
	int		_kf_ispare0;
	uint64_t	kf_cap_rights[2];	/* cap_rights_t */
	uint64_t	_kf_cap_spare;
	char		kf_path[PATH_MAX];
};

#define	KVME_TYPE_NONE		0
#define	KVME_TYPE_DEFAULT	1
#define	KVME_TYPE_VNODE		2
#define	KVME_TYPE_SWAP		3
#define	KVME_TYPE_DEVICE	4
#define	KVME_TYPE_PHYS		5
#define	KVME_TYPE_DEAD		6
#define	KVME_TYPE_SG		7
#define	KVME_TYPE_MGTDEVICE	8
#define	KVME_TYPE_GUARD		9
#define	KVME_TYPE_UNKNOWN	255

#define	KVME_PROT_READ		0x00000001
#define	KVME_PROT_WRITE		0x00000002
#define	KVME_PROT_EXEC		0x00000004
#define	KVME_MAX_PROT_READ	0x00010000
#define	KVME_MAX_PROT_WRITE	0x00020000
#define	KVME_MAX_PROT_EXEC	0x00040000

#define	KVME_FLAG_COW		0x00000001
#define	KVME_FLAG_NEEDS_COPY	0x00000002
#define	KVME_FLAG_NOCOREDUMP	0x00000004
#define	KVME_FLAG_SUPER		0x00000008
#define	KVME_FLAG_GROWS_UP	0x00000010
#define	KVME_FLAG_GROWS_DOWN	0x00000020
#define	KVME_FLAG_USER_WIRED	0x00000040
#define	KVME_FLAG_SYSVSHM	0x00000080
#define	KVME_FLAG_POSIXSHM	0x00000100

/*
 * kern.proc.vmmap packs the records: kve_structsize is the size of each
 * one, which ends after the NUL of kve_path, rounded up to 8 bytes.
 */
struct kinfo_vmentry {
	int		kve_structsize;
	int		kve_type;
	uint64_t	kve_start;
	uint64_t	kve_end;
	uint64_t	kve_offset;
	uint64_t	kve_vn_fileid;
	uint32_t	kve_vn_fsid_freebsd11;
	int		kve_flags;
	int		kve_resident;
	int		kve_private_resident;
	int		kve_protection;
	int		kve_ref_count;
	int		kve_shadow_count;
	int		kve_vn_type;
	uint64_t	kve_vn_size;
	uint32_t	kve_vn_rdev_freebsd11;
	uint16_t	kve_vn_mode;
	uint16_t	kve_status;
	union {
		uint64_t	_kve_vn_fsid;
		uint64_t	_kve_obj;
	} kve_type_spec;
	uint64_t	kve_vn_rdev;
	int		_kve_ispare[8];
	char		kve_path[PATH_MAX];
};
#define	kve_vn_fsid	kve_type_spec._kve_vn_fsid
#define	kve_obj		kve_type_spec._kve_obj
