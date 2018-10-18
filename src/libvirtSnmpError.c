/*
 * libvirtSnmpError.c: libvirt error handling function
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

#include <config.h>

#include <stdio.h>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "libvirtSnmpError.h"

/**
 * printLibvirtError:
 * @fmt: Error message format string
 *
 * Print and reset Libvirt error.
 */
void
printLibvirtError(const char *fmt, ...)
{
    const char *libvirtErr = virGetLastErrorMessage();
    char ebuf[1024];
    int rc;
    int size = 0;
    va_list ap;

    va_start(ap, fmt);
    rc = vsnprintf(ebuf, sizeof(ebuf), fmt, ap);
    size += rc;
    va_end(ap);

    if (rc < 0 || size >= sizeof(ebuf))
        return;

    rc = snprintf(ebuf + size, sizeof(ebuf) - size, ": %s\n", libvirtErr);
    size += rc;

    if (rc < 0 || size >= sizeof(ebuf))
        return;

    fputs(ebuf, stderr);
    snmp_log(LOG_ERR, "%s", ebuf);

    virResetLastError();
}


/**
 * printSystemError:
 * @theerrno: the errno value
 * @fmt: Error message format string
 *
 * Print system error with @theerrno translated into human readable form.
 */
void
printSystemError(int theerrno, const char *fmt, ...)
{
    char ebuf[1024];
    char sysebuf[1024];
    int rc;
    int size = 0;
    va_list ap;

    if (!strerror_r(theerrno, sysebuf, sizeof(sysebuf)))
        return;

    va_start(ap, fmt);
    rc = vsnprintf(ebuf, sizeof(ebuf), fmt, ap);
    size += rc;
    va_end(ap);

    if (rc < 0 || size >= sizeof(ebuf))
        return;

    rc = snprintf(ebuf + size, sizeof(ebuf) - size, ": %s\n", sysebuf);
    size += rc;

    if (rc < 0 || size >= sizeof(ebuf))
        return;

    fputs(ebuf, stderr);
    snmp_log(LOG_ERR, "%s", ebuf);
}
