/*
 * libvirtGuestTable_interface.h: SNMP table interface
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

/** @ingroup interface: Routines to interface to Net-SNMP
 *
 * \warning This code should not be modified, called directly,
 *          or used to interpret functionality. It is subject to
 *          change at any time.
 *
 * @{
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */
#ifndef LIBVIRTGUESTTABLE_INTERFACE_H
#define LIBVIRTGUESTTABLE_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "libvirtGuestTable.h"


/* ********************************************************************
 * Table declarations
 */

/* PUBLIC interface initialization routine */
void _libvirtGuestTable_initialize_interface(libvirtGuestTable_registration * user_ctx,
                                    u_long flags);
void _libvirtGuestTable_shutdown_interface(libvirtGuestTable_registration * user_ctx);

libvirtGuestTable_registration *
libvirtGuestTable_registration_get( void );

libvirtGuestTable_registration *
libvirtGuestTable_registration_set( libvirtGuestTable_registration * newreg );

netsnmp_container *libvirtGuestTable_container_get( void );
int libvirtGuestTable_container_size( void );

u_int libvirtGuestTable_dirty_get( void );
void libvirtGuestTable_dirty_set( u_int status );

    libvirtGuestTable_rowreq_ctx * libvirtGuestTable_allocate_rowreq_ctx(void *);
void libvirtGuestTable_release_rowreq_ctx(libvirtGuestTable_rowreq_ctx *rowreq_ctx);

int libvirtGuestTable_index_to_oid(netsnmp_index *oid_idx,
                            libvirtGuestTable_mib_index *mib_idx);
int libvirtGuestTable_index_from_oid(netsnmp_index *oid_idx,
                              libvirtGuestTable_mib_index *mib_idx);

/*
 * access to certain internals. use with caution!
 */
void libvirtGuestTable_valid_columns_set(netsnmp_column_info *vc);


#ifdef __cplusplus
}
#endif

#endif /* LIBVIRTGUESTTABLE_INTERFACE_H */
/** @} */
