/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Copyright (C) 2026 CactOS Project */

#ifndef CACT_WAYLAND_CONFIG_H
#define CACT_WAYLAND_CONFIG_H

/*
 * Wayland's build probes the platform; CactOS is a known target, so the answers
 * are written down instead of probed.  Note the sources test most of these with
 * `#ifdef`, so what CactOS does not have is deliberately left *undefined* rather
 * than defined as 0.
 */

#define PACKAGE         "wayland"
#define PACKAGE_VERSION "1.26.0"

/* CactLibc provides these. */
#define HAVE_ACCEPT4       1
#define HAVE_MKOSTEMP      1
#define HAVE_GETTID        1
#define HAVE_STRNDUP       1
/* memfd_create() plus F_ADD_SEALS/F_GET_SEALS and the seal enforcement in
 * ftruncate/write — CactOS grew both (the /dev/memfd device always existed;
 * the libc wrapper and the kernel seals are new), which is what Wayland's shm
 * pools want: they create the pool as a sealed memfd so a client cannot shrink
 * it out from under the compositor. */
#define HAVE_MEMFD_CREATE  1

/* Not present, and the sources want them undefined:
 *   HAVE_PRCTL, HAVE_POSIX_FALLOCATE, HAVE_MREMAP,
 *   HAVE_SYS_PRCTL_H, HAVE_SYS_PROCCTL_H, HAVE_SYS_UCRED_H */

/* Probes the sources read numerically. */
#define HAVE_BROKEN_MSG_CMSG_CLOEXEC 0
#define HAVE_XUCRED_CR_PID           0
#define HAVE_LIBXML                  0

#endif /* CACT_WAYLAND_CONFIG_H */
