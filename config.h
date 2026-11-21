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

/* Not present, and the sources want them undefined:
 *   HAVE_PRCTL, HAVE_POSIX_FALLOCATE, HAVE_MREMAP,
 *   HAVE_SYS_PRCTL_H, HAVE_SYS_PROCCTL_H, HAVE_SYS_UCRED_H
 *
 * HAVE_MEMFD_CREATE is deliberately off too: CactOS has the memfd *device*
 * (CACT_MEMFDCTL_CREATE on /dev/memfd) but CactLibc has no memfd_create()
 * wrapper and the kernel has no sealing (MFD_ALLOW_SEALING / F_ADD_SEALS /
 * F_SEAL_*), which is what Wayland's shm pools want it for.  With it undefined
 * Wayland takes its fallback anonymous-file path over /dev/shm (shm_open).
 * Adding memfd_create() + seals to CactLibc/CactKernel would be the better fix
 * and this define should then be turned back on. */

/* Probes the sources read numerically. */
#define HAVE_BROKEN_MSG_CMSG_CLOEXEC 0
#define HAVE_XUCRED_CR_PID           0
#define HAVE_LIBXML                  0

#endif /* CACT_WAYLAND_CONFIG_H */
