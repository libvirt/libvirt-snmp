/*
 * libvirtGuestTable_data_access.h: SNMP table data access routines
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

#ifndef LIBVIRTGUESTTABLE_DATA_ACCESS_H
#define LIBVIRTGUESTTABLE_DATA_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif


/* *********************************************************************
 * function declarations
 */

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table libvirtGuestTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * LIBVIRT-MIB::libvirtGuestTable is subid 1 of libvirtObjects.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.12345.1.1, length: 9
*/


    int libvirtGuestTable_init_data(libvirtGuestTable_registration * libvirtGuestTable_reg);


    /*
     * TODO:180:o: Review libvirtGuestTable cache timeout.
     * The number of seconds before the cache times out
     */
#define LIBVIRTGUESTTABLE_CACHE_TIMEOUT   5

void libvirtGuestTable_container_init(netsnmp_container **container_ptr_ptr,
                             netsnmp_cache *cache);
void libvirtGuestTable_container_shutdown(netsnmp_container *container_ptr);

int libvirtGuestTable_container_load(netsnmp_container *container);
void libvirtGuestTable_container_free(netsnmp_container *container);

int libvirtGuestTable_cache_load(netsnmp_container *container);
void libvirtGuestTable_cache_free(netsnmp_container *container);

    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
/* *********************************************************************
 * Since we have no idea how you really access your data, we'll go with
 * a worst case example: a flat text file.
 */
#define MAX_LINE_SIZE 256
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/
    int libvirtGuestTable_row_prep( libvirtGuestTable_rowreq_ctx *rowreq_ctx);

int libvirtGuestTable_validate_index( libvirtGuestTable_registration * libvirtGuestTable_reg,
                               libvirtGuestTable_rowreq_ctx *rowreq_ctx);
int libvirtGuestUUID_check_index( libvirtGuestTable_rowreq_ctx *rowreq_ctx ); /* internal */


#ifdef __cplusplus
}
#endif

#endif /* LIBVIRTGUESTTABLE_DATA_ACCESS_H */
