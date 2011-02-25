/*
 * libvirtNotifications.h: Fill in trap packets
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * See COPYING for the license of this software
 *
 * Michal Privoznik <mprivozn@redhat.com>
 */

#ifndef LIBVIRTNOTIFICATIONS_H
#define LIBVIRTNOTIFICATIONS_H

#include "libvirtSnmp.h"

/*
 * function declarations
 */
int send_libvirtGuestNotif_trap(virDomainPtr dom);

#endif /* LIBVIRTNOTIFICATIONS_H */
