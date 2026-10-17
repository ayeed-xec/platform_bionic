/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef _BITS_TERMIOS_INLINES_H_
#define _BITS_TERMIOS_INLINES_H_

#include <sys/cdefs.h>

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <bits/freebsd/termios.h>

#if !defined(__BIONIC_TERMIOS_INLINE)
#define __BIONIC_TERMIOS_INLINE static __inline
#endif

__BEGIN_DECLS

// FreeBSD keeps separate input and output speeds in struct termios, and a
// speed is the baud rate itself. As on Linux, only the Bxxx values are
// accepted.
static __inline int __bionic_termios_valid_speed(speed_t speed) {
  switch (speed) {
    case B0: case B50: case B75: case B110: case B134: case B150: case B200:
    case B300: case B600: case B1200: case B1800: case B2400: case B4800:
    case B7200: case B9600: case B14400: case B19200: case B28800:
    case B38400: case B57600: case B76800: case B115200: case B230400:
    case B460800: case B500000: case B921600: case B1000000: case B1500000:
    case B2000000: case B2500000: case B3000000: case B3500000: case B4000000:
      return 1;
  }
  return 0;
}

__BIONIC_TERMIOS_INLINE speed_t cfgetispeed(const struct termios* _Nonnull s) {
  return s->c_ispeed;
}

__BIONIC_TERMIOS_INLINE speed_t cfgetospeed(const struct termios* _Nonnull s) {
  return s->c_ospeed;
}

__BIONIC_TERMIOS_INLINE void cfmakeraw(struct termios* _Nonnull s) {
  s->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
  s->c_oflag &= ~OPOST;
  s->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
  s->c_cflag &= ~(CSIZE|PARENB);
  s->c_cflag |= CS8;
  s->c_cc[VMIN] = 1;
  s->c_cc[VTIME] = 0;
}

__BIONIC_TERMIOS_INLINE int cfsetispeed(struct termios* _Nonnull s, speed_t speed) {
  if (!__bionic_termios_valid_speed(speed)) {
    errno = EINVAL;
    return -1;
  }
  s->c_ispeed = speed;
  return 0;
}

__BIONIC_TERMIOS_INLINE int cfsetospeed(struct termios* _Nonnull s, speed_t speed) {
  if (!__bionic_termios_valid_speed(speed)) {
    errno = EINVAL;
    return -1;
  }
  s->c_ospeed = speed;
  return 0;
}

__BIONIC_TERMIOS_INLINE int cfsetspeed(struct termios* _Nonnull s, speed_t speed) {
  if (!__bionic_termios_valid_speed(speed)) {
    errno = EINVAL;
    return -1;
  }
  s->c_ispeed = s->c_ospeed = speed;
  return 0;
}

__BIONIC_TERMIOS_INLINE int tcdrain(int fd) {
  return ioctl(fd, TIOCDRAIN, 0);
}

__BIONIC_TERMIOS_INLINE int tcflow(int fd, int action) {
  struct termios t;
  cc_t c;

  switch (action) {
    case TCOOFF: return ioctl(fd, TIOCSTOP, 0);
    case TCOON: return ioctl(fd, TIOCSTART, 0);
    case TCIOFF:
    case TCION:
      // Send the STOP or the START character to the terminal.
      if (ioctl(fd, TIOCGETA, &t) == -1) return -1;
      c = t.c_cc[action == TCIOFF ? VSTOP : VSTART];
      if (c != _POSIX_VDISABLE && write(fd, &c, sizeof(c)) == -1) return -1;
      return 0;
    default: errno = EINVAL; return -1;
  }
}

__BIONIC_TERMIOS_INLINE int tcflush(int fd, int queue) {
  // TCIFLUSH, TCOFLUSH and TCIOFLUSH line up with the FREAD, FWRITE and
  // FREAD|FWRITE bits that TIOCFLUSH wants.
  if (queue != TCIFLUSH && queue != TCOFLUSH && queue != TCIOFLUSH) {
    errno = EINVAL;
    return -1;
  }
  return ioctl(fd, TIOCFLUSH, &queue);
}

__BIONIC_TERMIOS_INLINE int tcgetattr(int fd, struct termios* _Nonnull s) {
  return ioctl(fd, TIOCGETA, s);
}

__BIONIC_TERMIOS_INLINE pid_t tcgetsid(int fd) {
  pid_t sid;
  return (ioctl(fd, TIOCGSID, &sid) == -1) ? -1 : sid;
}

__BIONIC_TERMIOS_INLINE int tcsendbreak(int fd, int duration) {
  // Duration 0 means 0.4 seconds (POSIX allows 0.25 to 0.5). Anything else
  // is in tenths of a second, like Linux's TCSBRKP.
  long ms = (duration > 0) ? duration * 100L : 400L;
  struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

  if (ioctl(fd, TIOCSBRK, 0) == -1) return -1;
  while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
  }
  return ioctl(fd, TIOCCBRK, 0);
}

__BIONIC_TERMIOS_INLINE int tcsetattr(int fd, int optional_actions, const struct termios* _Nonnull s) {
  unsigned long cmd;
  switch (optional_actions) {
    case TCSANOW: cmd = TIOCSETA; break;
    case TCSADRAIN: cmd = TIOCSETAW; break;
    case TCSAFLUSH: cmd = TIOCSETAF; break;
    default: errno = EINVAL; return -1;
  }
  return ioctl(fd, cmd, s);
}

__END_DECLS

#endif
