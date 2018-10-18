/*
 * libvirtSnmp.h: interface between libvirt and agent
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Michal Privoznik <mprivozn@redhat.com>
 */

#ifndef __LIBVIRT_SNMP_H__
# define __LIBVIRT_SNMP_H__

/* standard libvirt includes */
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * Populate libvirtGuestTable into given container.
 */
extern int
libvirtSnmpLoadGuests(netsnmp_container *container);

extern int
libvirtSnmpInit(void);

extern void
libvirtSnmpShutdown(void);

/**
 * Check that domain with given UUID exists.
 * Return 0 if so, -1 if not.
 */
extern int
libvirtSnmpCheckDomainExists(unsigned char *uuid);

extern int
libvirtSnmpCreate(unsigned char *uuid, int state);

extern int
libvirtSnmpDestroy(unsigned char *uuid);

extern int
libvirtSnmpChangeState(unsigned char *uuid, int newstate, int oldstate);

#endif /* __LIBVIRT_SNMP_H__ */

