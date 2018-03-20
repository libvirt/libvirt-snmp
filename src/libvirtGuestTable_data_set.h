/*
 * libvirtGuestTable_data_set.h: SNMP table data set routines
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

#ifndef LIBVIRTGUESTTABLE_DATA_SET_H
#define LIBVIRTGUESTTABLE_DATA_SET_H

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************************
 * SET function declarations
 */

/* *********************************************************************
 * SET Table declarations
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
 * OID: .1.3.6.1.4.1.36957.1.1, length: 9
*/


int libvirtGuestTable_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx);
int libvirtGuestTable_undo_cleanup( libvirtGuestTable_rowreq_ctx *rowreq_ctx);
int libvirtGuestTable_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx);
int libvirtGuestTable_commit( libvirtGuestTable_rowreq_ctx *rowreq_ctx);
int libvirtGuestTable_undo_commit( libvirtGuestTable_rowreq_ctx *rowreq_ctx);


int libvirtGuestName_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, char *libvirtGuestName_val_ptr,  size_t libvirtGuestName_val_ptr_len);
int libvirtGuestName_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestName_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, char *libvirtGuestName_val_ptr,  size_t libvirtGuestName_val_ptr_len );
int libvirtGuestName_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );

int libvirtGuestState_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestState_val);
int libvirtGuestState_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestState_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestState_val );
int libvirtGuestState_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );

int libvirtGuestCpuCount_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestCpuCount_val);
int libvirtGuestCpuCount_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestCpuCount_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestCpuCount_val );
int libvirtGuestCpuCount_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );

int libvirtGuestMemoryCurrent_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestMemoryCurrent_val);
int libvirtGuestMemoryCurrent_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestMemoryCurrent_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestMemoryCurrent_val );
int libvirtGuestMemoryCurrent_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );

int libvirtGuestMemoryLimit_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestMemoryLimit_val);
int libvirtGuestMemoryLimit_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestMemoryLimit_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestMemoryLimit_val );
int libvirtGuestMemoryLimit_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );

int libvirtGuestCpuTime_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, struct counter64 libvirtGuestCpuTime_val);
int libvirtGuestCpuTime_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestCpuTime_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, struct counter64 libvirtGuestCpuTime_val );
int libvirtGuestCpuTime_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );

int libvirtGuestRowStatus_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestRowStatus_val);
int libvirtGuestRowStatus_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx );
int libvirtGuestRowStatus_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestRowStatus_val );
int libvirtGuestRowStatus_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx );


int libvirtGuestTable_check_dependencies(libvirtGuestTable_rowreq_ctx *ctx);


#ifdef __cplusplus
}
#endif

#endif /* LIBVIRTGUESTTABLE_DATA_SET_H */
