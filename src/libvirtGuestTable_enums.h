/*
 * libvirtGuestTable_enums.h: SNMP table enums
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

#ifndef LIBVIRTGUESTTABLE_ENUMS_H
#define LIBVIRTGUESTTABLE_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

 /*
 * NOTES on enums
 * ==============
 *
 * Value Mapping
 * -------------
 * If the values for your data type don't exactly match the
 * possible values defined by the mib, you should map them
 * below. For example, a boolean flag (1/0) is usually represented
 * as a TruthValue in a MIB, which maps to the values (1/2).
 *
 */
/*************************************************************************
 *************************************************************************
 *
 * enum definitions for table libvirtGuestTable
 *
 *************************************************************************
 *************************************************************************/

/*************************************************************
 * constants for enums for the MIB node
 * libvirtGuestState (INTEGER / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef LIBVIRTGUESTSTATE_ENUMS
#define LIBVIRTGUESTSTATE_ENUMS

#define LIBVIRTGUESTSTATE_RUNNING  1 
#define LIBVIRTGUESTSTATE_BLOCKED  2 
#define LIBVIRTGUESTSTATE_PAUSED  3 
#define LIBVIRTGUESTSTATE_SHUTDOWN  4 
#define LIBVIRTGUESTSTATE_SHUTOFF  5 
#define LIBVIRTGUESTSTATE_CRASHED  6 

#endif /* LIBVIRTGUESTSTATE_ENUMS */


/*************************************************************
 * constants for enums for the MIB node
 * libvirtGuestRowStatus (RowStatus / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef ROWSTATUS_ENUMS
#define ROWSTATUS_ENUMS

#define ROWSTATUS_ACTIVE  1 
#define ROWSTATUS_NOTINSERVICE  2 
#define ROWSTATUS_NOTREADY  3 
#define ROWSTATUS_CREATEANDGO  4 
#define ROWSTATUS_CREATEANDWAIT  5 
#define ROWSTATUS_DESTROY  6 

#endif /* ROWSTATUS_ENUMS */




#ifdef __cplusplus
}
#endif

#endif /* LIBVIRTGUESTTABLE_ENUMS_H */
