/*
 * libvirtNotifications.c: Fill in trap packets
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

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <string.h>
#include "libvirtNotifications.h"
#include "libvirtGuestTable_enums.h"
#include "libvirtSnmpError.h"

static const oid sysuptime_oid[] = { 1, 3, 6, 1, 2, 1, 1, 3, 0 };
static const oid snmptrap_oid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };

int
send_libvirtGuestNotif_trap(virDomainPtr dom)
{
    netsnmp_variable_list *var_list = NULL;
    const oid libvirtGuestNotif_oid[] = { 1, 3, 6, 1, 4, 1, 36957, 0, 1 };
    const oid libvirtGuestName_oid[] =
        { 1, 3, 6, 1, 4, 1, 36957, 1, 1, 1, 2, 0 };
    const oid libvirtGuestUUID_oid[] =
        { 1, 3, 6, 1, 4, 1, 36957, 1, 1, 1, 1, 0 };
    const oid libvirtGuestState_oid[] =
        { 1, 3, 6, 1, 4, 1, 36957, 1, 1, 1, 3, 0 };
    const oid libvirtGuestRowStatus_oid[] =
        { 1, 3, 6, 1, 4, 1, 36957, 1, 1, 1, 9, 0 };


    const char *domName = virDomainGetName(dom);
    unsigned char domUUID[VIR_UUID_BUFLEN];
    virDomainInfo info;
    int rowstatus = ROWSTATUS_ACTIVE;
    u_long uptime = netsnmp_get_agent_uptime();

    if (virDomainGetUUID(dom, domUUID) < 0) {
        printLibvirtError("Failed to get domain UUID");
        return SNMP_ERR_GENERR;
    }

    if (virDomainGetInfo(dom, &info) < 0) {
        printLibvirtError("Failed to get domain info");
        return SNMP_ERR_GENERR;
    }

    /*
     * If domain is shutting down, row in libvirtGuestTable will
     * not be accessible anymore.
     */
    switch (info.state) {
        case VIR_DOMAIN_SHUTDOWN:
        case VIR_DOMAIN_SHUTOFF:
        case VIR_DOMAIN_CRASHED:
            rowstatus = ROWSTATUS_NOTINSERVICE;
            break;

        default:
            rowstatus = ROWSTATUS_ACTIVE;
            break;
    };

    /*
     * Set the sysUptime.0 value
     */
    snmp_varlist_add_variable(&var_list,
                              sysuptime_oid, OID_LENGTH(sysuptime_oid),
                              ASN_TIMETICKS,
                              (u_char*)&uptime,
                              sizeof(uptime));

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
                              snmptrap_oid, OID_LENGTH(snmptrap_oid),
                              ASN_OBJECT_ID,
                              libvirtGuestNotif_oid,
                              sizeof(libvirtGuestNotif_oid));

    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
                              libvirtGuestName_oid,
                              OID_LENGTH(libvirtGuestName_oid),
                              ASN_OCTET_STR, domName, strlen(domName));
    snmp_varlist_add_variable(&var_list,
                              libvirtGuestUUID_oid,
                              OID_LENGTH(libvirtGuestUUID_oid),
                              ASN_OCTET_STR, domUUID, sizeof(domUUID));
    snmp_varlist_add_variable(&var_list,
                              libvirtGuestState_oid,
                              OID_LENGTH(libvirtGuestState_oid),
                              ASN_INTEGER,
                              (u_char *) & info.state, sizeof(info.state));
    snmp_varlist_add_variable(&var_list,
                              libvirtGuestRowStatus_oid,
                              OID_LENGTH(libvirtGuestRowStatus_oid),
                              ASN_INTEGER,
                              (u_char *) & rowstatus, sizeof(rowstatus));

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     * and clean up
     */
    send_v2trap(var_list);
    snmp_free_varbind(var_list);

    return SNMP_ERR_NOERROR;
}
