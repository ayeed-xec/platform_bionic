/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <net/if.h>

#include <errno.h>
#include <ifaddrs.h>
#if !defined(__FreeBSD__)
#include <linux/if_packet.h>
#endif
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#if !defined(__FreeBSD__)
#include <linux/sockios.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#if defined(__FreeBSD__)
#include <sys/sysctl.h>
#endif
#include <unistd.h>

#include "private/ScopedFd.h"
#if defined(__FreeBSD__)
#include "private/freebsd_if_mib.h"
#endif

#include "bionic_netlink.h"

#if defined(__FreeBSD__)
// FreeBSD has no SIOCGIFNAME. The interface MIB gives the name of an index
// in one sysctl, in the vnet of the caller.
char* if_indextoname(unsigned ifindex, char* ifname) {
  int mib[] = {CTL_NET, PF_LINK, FREEBSD_NETLINK_GENERIC, FREEBSD_IFMIB_IFDATA,
               static_cast<int>(ifindex), FREEBSD_IFDATA_GENERAL};
  freebsd_ifmibdata ifmd;
  size_t size = sizeof(ifmd);
  if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), &ifmd, &size, nullptr, 0) == -1) {
    // Linux reports an unknown index as ENODEV.
    if (errno == ENOENT) errno = ENODEV;
    return nullptr;
  }

  strlcpy(ifname, ifmd.ifmd_name, IF_NAMESIZE);
  return ifname;
}
#else
char* if_indextoname(unsigned ifindex, char* ifname) {
  ScopedFd s(socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, 0));
  if (s.get() == -1) return nullptr;

  ifreq ifr = {.ifr_ifindex = static_cast<int>(ifindex)};
  if (ioctl(s.get(), SIOCGIFNAME, &ifr) == -1) return nullptr;

  strcpy(ifname, ifr.ifr_name);
  return ifname;
}
#endif

unsigned if_nametoindex(const char* ifname) {
  ifreq ifr = {};

  if (strlen(ifname) >= sizeof(ifr.ifr_name)) {
    errno = ENODEV;
    return 0;
  }
  strcpy(ifr.ifr_name, ifname);

  ScopedFd s(socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, 0));
  if (s.get() == -1) return 0;

#if defined(__FreeBSD__)
  if (ioctl(s.get(), SIOCGIFINDEX, &ifr) == -1) {
    // Linux reports an unknown interface as ENODEV, FreeBSD as ENXIO.
    if (errno == ENXIO) errno = ENODEV;
    return 0;
  }
  return static_cast<unsigned short>(ifr.ifr_index);
#else
  return (ioctl(s.get(), SIOCGIFINDEX, &ifr) == -1) ? 0 : ifr.ifr_ifindex;
#endif
}

struct if_list {
  if_list* next;
  struct if_nameindex data;

  explicit if_list(if_list** list) {
    // push_front onto `list`.
    next = *list;
    *list = this;
  }

  static void Free(if_list* list, bool names_too) {
    while (list) {
      if_list* it = list;
      list = it->next;
      if (names_too) free(it->data.if_name);
      free(it);
    }
  }
};

static void __if_nameindex_callback(void* context, nlmsghdr* hdr) {
  if_list** list = reinterpret_cast<if_list**>(context);
  if (hdr->nlmsg_type == RTM_NEWLINK) {
    ifinfomsg* ifi = reinterpret_cast<ifinfomsg*>(NLMSG_DATA(hdr));

    // Create a new entry and set the interface index.
    if_list* new_link = new if_list(list);
    new_link->data.if_index = ifi->ifi_index;

    // Go through the various bits of information and find the name.
    rtattr* rta = IFLA_RTA(ifi);
    size_t rta_len = IFLA_PAYLOAD(hdr);
    while (RTA_OK(rta, rta_len)) {
      if (rta->rta_type == IFLA_IFNAME) {
        new_link->data.if_name = strndup(reinterpret_cast<char*>(RTA_DATA(rta)), RTA_PAYLOAD(rta));
      }
      rta = RTA_NEXT(rta, rta_len);
    }
  }
}

struct if_nameindex* if_nameindex() {
  if_list* list = nullptr;

  // Open the netlink socket and ask for all the links;
  NetlinkConnection nc;
  bool okay = nc.SendRequest(RTM_GETLINK) && nc.ReadResponses(__if_nameindex_callback, &list);
  if (!okay) {
    if_list::Free(list, true);
    return nullptr;
  }

  // Count the interfaces.
  size_t interface_count = 0;
  for (if_list* it = list; it != nullptr; it = it->next) {
    ++interface_count;
  }

  // Build the array POSIX requires us to return.
  struct if_nameindex* result = new struct if_nameindex[interface_count + 1];
  if (result) {
    struct if_nameindex* out = result;
    for (if_list* it = list; it != nullptr; it = it->next) {
      out->if_index = it->data.if_index;
      out->if_name = it->data.if_name;
      ++out;
    }
    out->if_index = 0;
    out->if_name = nullptr;
  }

  // Free temporary storage.
  if_list::Free(list, false);

  return result;
}

void if_freenameindex(struct if_nameindex* array) {
  if (array == nullptr) return;

  struct if_nameindex* ptr = array;
  while (ptr->if_index != 0 || ptr->if_name != nullptr) {
    free(ptr->if_name);
    ++ptr;
  }

  delete[] array;
}
