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

// The credential options of AF_UNIX sockets, capget(2) and capset(2), and
// inotify.  bionic-freebsd maps each of them to a FreeBSD call.

#include <gtest/gtest.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/capability.h>
#include <sys/inotify.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/unique_fd.h>

#include "utils.h"

using android::base::unique_fd;

// Send one byte, with the descriptor passed in SCM_RIGHTS when it is not -1.
static void SendByte(int fd, int passed) {
  char data = 'x';
  iovec iov = {&data, 1};
  alignas(cmsghdr) char control[CMSG_SPACE(sizeof(int))] = {};
  msghdr msg = {};
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  if (passed != -1) {
    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);
    cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(cmsg), &passed, sizeof(int));
  }
  ASSERT_EQ(1, sendmsg(fd, &msg, 0));
}

static void CheckCredentials(const cmsghdr* cmsg) {
  ASSERT_NE(nullptr, cmsg);
  ASSERT_EQ(SOL_SOCKET, cmsg->cmsg_level);
  ASSERT_EQ(SCM_CREDENTIALS, cmsg->cmsg_type);
  ASSERT_EQ(CMSG_LEN(sizeof(ucred)), cmsg->cmsg_len);
  ucred cred;
  memcpy(&cred, CMSG_DATA(cmsg), sizeof(cred));
  ASSERT_EQ(getpid(), cred.pid);
  ASSERT_EQ(getuid(), cred.uid);
  ASSERT_EQ(getgid(), cred.gid);
}

static void PassCredPair(int type, unique_fd* sender, unique_fd* receiver) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, type | SOCK_CLOEXEC, 0, fds));
  sender->reset(fds[0]);
  receiver->reset(fds[1]);
  int on = 1;
  ASSERT_EQ(0, setsockopt(*receiver, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)));
  int value = 0;
  socklen_t len = sizeof(value);
  ASSERT_EQ(0, getsockopt(*receiver, SOL_SOCKET, SO_PASSCRED, &value, &len));
  ASSERT_EQ(1, value);
}

// The receive buffer has room only for the struct ucred, as logd's has.
static void ReceiveCredentials(int type) {
  unique_fd sender, receiver;
  ASSERT_NO_FATAL_FAILURE(PassCredPair(type, &sender, &receiver));
  ASSERT_NO_FATAL_FAILURE(SendByte(sender, -1));

  char data;
  iovec iov = {&data, 1};
  alignas(cmsghdr) char control[CMSG_SPACE(sizeof(ucred))];
  msghdr msg = {};
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = control;
  msg.msg_controllen = sizeof(control);
  ASSERT_EQ(1, recvmsg(receiver, &msg, 0));
  ASSERT_EQ(0, msg.msg_flags & MSG_CTRUNC);
  ASSERT_NO_FATAL_FAILURE(CheckCredentials(CMSG_FIRSTHDR(&msg)));
  ASSERT_EQ(nullptr, CMSG_NXTHDR(&msg, CMSG_FIRSTHDR(&msg)));
}

TEST(sys_socket_cred, so_passcred_dgram) {
  ReceiveCredentials(SOCK_DGRAM);
}

TEST(sys_socket_cred, so_passcred_seqpacket) {
  ReceiveCredentials(SOCK_SEQPACKET);
}

TEST(sys_socket_cred, so_passcred_stream) {
  ReceiveCredentials(SOCK_STREAM);
}

TEST(sys_socket_cred, so_passcred_with_rights) {
  unique_fd sender, receiver;
  ASSERT_NO_FATAL_FAILURE(PassCredPair(SOCK_DGRAM, &sender, &receiver));
  unique_fd passed(open("/dev/null", O_RDONLY | O_CLOEXEC));
  ASSERT_NE(-1, passed.get());
  ASSERT_NO_FATAL_FAILURE(SendByte(sender, passed));

  char data;
  iovec iov = {&data, 1};
  alignas(cmsghdr) char control[CMSG_SPACE(sizeof(ucred)) + CMSG_SPACE(sizeof(int))];
  msghdr msg = {};
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = control;
  msg.msg_controllen = sizeof(control);
  ASSERT_EQ(1, recvmsg(receiver, &msg, 0));
  ASSERT_EQ(0, msg.msg_flags & MSG_CTRUNC);
  int credentials = 0;
  int rights = 0;
  for (cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg != nullptr; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
    if (cmsg->cmsg_type == SCM_CREDENTIALS) {
      ASSERT_NO_FATAL_FAILURE(CheckCredentials(cmsg));
      credentials++;
    } else {
      ASSERT_EQ(SCM_RIGHTS, cmsg->cmsg_type);
      ASSERT_EQ(CMSG_LEN(sizeof(int)), cmsg->cmsg_len);
      int fd;
      memcpy(&fd, CMSG_DATA(cmsg), sizeof(fd));
      ASSERT_EQ(0, close(fd));
      rights++;
    }
  }
  ASSERT_EQ(1, credentials);
  ASSERT_EQ(1, rights);
}

// A descriptor that does not fit in the buffer is closed, not leaked.
TEST(sys_socket_cred, so_passcred_rights_truncated) {
  unique_fd sender, receiver;
  ASSERT_NO_FATAL_FAILURE(PassCredPair(SOCK_DGRAM, &sender, &receiver));
  unique_fd passed(open("/dev/null", O_RDONLY | O_CLOEXEC));
  ASSERT_NE(-1, passed.get());
  ASSERT_NO_FATAL_FAILURE(SendByte(sender, passed));
  int lowest = open("/dev/null", O_RDONLY | O_CLOEXEC);
  ASSERT_NE(-1, lowest);
  ASSERT_EQ(0, close(lowest));

  char data;
  iovec iov = {&data, 1};
  alignas(cmsghdr) char control[CMSG_SPACE(sizeof(ucred))];
  msghdr msg = {};
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = control;
  msg.msg_controllen = sizeof(control);
  ASSERT_EQ(1, recvmsg(receiver, &msg, 0));
  ASSERT_EQ(MSG_CTRUNC, msg.msg_flags & MSG_CTRUNC);
  for (cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg != nullptr; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
    ASSERT_NE(SCM_RIGHTS, cmsg->cmsg_type);
  }
  unique_fd probe(open("/dev/null", O_RDONLY | O_CLOEXEC));
  ASSERT_EQ(lowest, probe.get());
}

TEST(sys_socket_cred, so_peercred) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, fds));
  unique_fd a(fds[0]), b(fds[1]);
  ucred cred = {};
  socklen_t len = sizeof(cred);
  ASSERT_EQ(0, getsockopt(a, SOL_SOCKET, SO_PEERCRED, &cred, &len));
  ASSERT_EQ(sizeof(cred), len);
  ASSERT_EQ(getpid(), cred.pid);
  ASSERT_EQ(geteuid(), cred.uid);
  ASSERT_EQ(getegid(), cred.gid);
}

TEST(sys_socket_cred, so_peergroups) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, fds));
  unique_fd a(fds[0]), b(fds[1]);
  int n = getgroups(0, nullptr);
  ASSERT_NE(-1, n);
  std::vector<gid_t> expected(n);
  ASSERT_EQ(n, getgroups(n, expected.data()));

  // A short buffer gives ERANGE and the size that the groups need.
  socklen_t len = 0;
  if (n > 0) {
    ASSERT_EQ(-1, getsockopt(a, SOL_SOCKET, SO_PEERGROUPS, nullptr, &len));
    ASSERT_ERRNO(ERANGE);
    ASSERT_EQ(n * sizeof(gid_t), len);
  }
  std::vector<gid_t> groups(n + 1);
  len = groups.size() * sizeof(gid_t);
  ASSERT_EQ(0, getsockopt(a, SOL_SOCKET, SO_PEERGROUPS, groups.data(), &len));
  ASSERT_EQ(n * sizeof(gid_t), len);
  groups.resize(n);
  std::sort(expected.begin(), expected.end());
  std::sort(groups.begin(), groups.end());
  ASSERT_EQ(expected, groups);
}

TEST(sys_capability, capget_capset) {
  __user_cap_header_struct header = {_LINUX_CAPABILITY_VERSION_3, 0};
  __user_cap_data_struct data[_LINUX_CAPABILITY_U32S_3];
  ASSERT_EQ(0, capget(&header, data));
  for (const auto& d : data) {
    ASSERT_EQ(d.effective, d.permitted);
    if (geteuid() != 0) ASSERT_EQ(0U, d.effective);
  }
  // The sets that capget() gives can be set again.
  ASSERT_EQ(0, capset(&header, data));
  // A null data pointer only checks the version.
  ASSERT_EQ(0, capget(&header, nullptr));
}

TEST(sys_capability, capget_bad_version) {
  __user_cap_header_struct header = {0, 0};
  __user_cap_data_struct data[_LINUX_CAPABILITY_U32S_3];
  ASSERT_EQ(-1, capget(&header, data));
  ASSERT_ERRNO(EINVAL);
  ASSERT_EQ(static_cast<uint32_t>(_LINUX_CAPABILITY_VERSION_3), header.version);
}

TEST(sys_capability, capset_raise) {
  if (geteuid() == 0) GTEST_SKIP() << "an unprivileged process cannot raise a capability";
  __user_cap_header_struct header = {_LINUX_CAPABILITY_VERSION_3, 0};
  __user_cap_data_struct data[_LINUX_CAPABILITY_U32S_3] = {};
  data[CAP_TO_INDEX(CAP_KILL)].effective = CAP_TO_MASK(CAP_KILL);
  data[CAP_TO_INDEX(CAP_KILL)].permitted = CAP_TO_MASK(CAP_KILL);
  ASSERT_EQ(-1, capset(&header, data));
  ASSERT_ERRNO(EPERM);
}

TEST(sys_inotify, create_event) {
  TemporaryDir dir;
  unique_fd fd(inotify_init1(IN_CLOEXEC | IN_NONBLOCK));
  ASSERT_NE(-1, fd.get());
  ASSERT_EQ(FD_CLOEXEC, fcntl(fd, F_GETFD) & FD_CLOEXEC);
  ASSERT_EQ(O_NONBLOCK, fcntl(fd, F_GETFL) & O_NONBLOCK);
  int wd = inotify_add_watch(fd, dir.path, IN_CREATE);
  ASSERT_NE(-1, wd);

  std::string path = std::string(dir.path) + "/file";
  unique_fd file(open(path.c_str(), O_CREAT | O_WRONLY | O_CLOEXEC, 0600));
  ASSERT_NE(-1, file.get());
  alignas(inotify_event) char buf[sizeof(inotify_event) + NAME_MAX + 1];
  ssize_t n = read(fd, buf, sizeof(buf));
  ASSERT_GE(n, static_cast<ssize_t>(sizeof(inotify_event)));
  const inotify_event* event = reinterpret_cast<const inotify_event*>(buf);
  ASSERT_EQ(wd, event->wd);
  ASSERT_EQ(static_cast<uint32_t>(IN_CREATE), event->mask & IN_CREATE);
  ASSERT_STREQ("file", event->name);

  ASSERT_EQ(0, inotify_rm_watch(fd, wd));
  ASSERT_EQ(0, unlink(path.c_str()));
}

TEST(sys_inotify, init) {
  unique_fd fd(inotify_init());
  ASSERT_NE(-1, fd.get());
  ASSERT_EQ(0, fcntl(fd, F_GETFD) & FD_CLOEXEC);
}
