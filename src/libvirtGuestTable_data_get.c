/*
 * libvirtGuestTable_data_get.c: SNMP table data get routines
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

/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "libvirtGuestTable.h"


/** @defgroup data_get data_get: Routines to get data
 *
 * TODO:230:M: Implement libvirtGuestTable get routines.
 * TODO:240:M: Implement libvirtGuestTable mapping routines (if any).
 *
 * These routine are used to get the value for individual objects. The
 * row context is passed, along with a pointer to the memory where the
 * value should be copied.
 *
 * @{
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

/* ---------------------------------------------------------------------
 * TODO:200:r: Implement libvirtGuestTable data context functions.
 */


/**
 * set mib index(es)
 *
 * @param tbl_idx mib index structure
 * @param libvirtGuestUUID_ptr
 * @param libvirtGuestUUID_ptr_len
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This convenience function is useful for setting all the MIB index
 *  components with a single function call. It is assume that the C values
 *  have already been mapped from their native/rawformat to the MIB format.
 */
int
libvirtGuestTable_indexes_set_tbl_idx(libvirtGuestTable_mib_index *tbl_idx, char *libvirtGuestUUID_val_ptr,  size_t libvirtGuestUUID_val_ptr_len)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_indexes_set_tbl_idx","called\n"));

    /* libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H */
    tbl_idx->libvirtGuestUUID_len = sizeof(tbl_idx->libvirtGuestUUID)/sizeof(tbl_idx->libvirtGuestUUID[0]); /* max length */
    /*
     * make sure there is enough space for libvirtGuestUUID data
     */
    if (tbl_idx->libvirtGuestUUID_len < (libvirtGuestUUID_val_ptr_len)) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->libvirtGuestUUID_len = libvirtGuestUUID_val_ptr_len;
    memcpy( tbl_idx->libvirtGuestUUID, libvirtGuestUUID_val_ptr, libvirtGuestUUID_val_ptr_len* sizeof(libvirtGuestUUID_val_ptr[0]) );
    

    return MFD_SUCCESS;
} /* libvirtGuestTable_indexes_set_tbl_idx */

/**
 * @internal
 * set row context indexes
 *
 * @param reqreq_ctx the row context that needs updated indexes
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This function sets the mib indexs, then updates the oid indexs
 *  from the mib index.
 */
int
libvirtGuestTable_indexes_set(libvirtGuestTable_rowreq_ctx *rowreq_ctx, char *libvirtGuestUUID_val_ptr,  size_t libvirtGuestUUID_val_ptr_len)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_indexes_set","called\n"));

    if(MFD_SUCCESS != libvirtGuestTable_indexes_set_tbl_idx(&rowreq_ctx->tbl_idx
                                   , libvirtGuestUUID_val_ptr, libvirtGuestUUID_val_ptr_len
           ))
        return MFD_ERROR;

    /*
     * convert mib index to oid index
     */
    rowreq_ctx->oid_idx.len = sizeof(rowreq_ctx->oid_tmp) / sizeof(oid);
    if(0 != libvirtGuestTable_index_to_oid(&rowreq_ctx->oid_idx,
                                    &rowreq_ctx->tbl_idx)) {
        return MFD_ERROR;
    }

    return MFD_SUCCESS;
} /* libvirtGuestTable_indexes_set */


/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestName
 * libvirtGuestName is subid 2 of libvirtGuestEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.12345.1.1.1.2
 * Description:
Name of active virtual guest.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is OCTETSTR (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.
 */
/**
 * Extract the current value of the libvirtGuestName data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestName_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param libvirtGuestName_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by libvirtGuestName.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*libvirtGuestName_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update libvirtGuestName_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
libvirtGuestName_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, char **libvirtGuestName_val_ptr_ptr, size_t *libvirtGuestName_val_ptr_len_ptr )
{
	size_t len;
   /** we should have a non-NULL pointer and enough storage */
   netsnmp_assert( (NULL != libvirtGuestName_val_ptr_ptr) && (NULL != *libvirtGuestName_val_ptr_ptr));
   netsnmp_assert( NULL != libvirtGuestName_val_ptr_len_ptr );


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestName_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the libvirtGuestName data.
 * copy (* libvirtGuestName_val_ptr_ptr ) data and (* libvirtGuestName_val_ptr_len_ptr ) from rowreq_ctx->data
 */
    /*
     * make sure there is enough space for libvirtGuestName data
     */
    if (!rowreq_ctx->data.libvirtGuestName)
    	return MFD_SKIP;

    len = strlen(rowreq_ctx->data.libvirtGuestName) + 1;
    if ((NULL == (* libvirtGuestName_val_ptr_ptr )) ||
        ((* libvirtGuestName_val_ptr_len_ptr ) < len)) {
        /*
         * allocate space for libvirtGuestName data
         */
        (* libvirtGuestName_val_ptr_ptr ) = malloc(len);
        if(NULL == (* libvirtGuestName_val_ptr_ptr )) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return MFD_ERROR;
        }
    }
    (* libvirtGuestName_val_ptr_len_ptr ) = len;
    memcpy( (* libvirtGuestName_val_ptr_ptr ),
    		rowreq_ctx->data.libvirtGuestName, len );

    return MFD_SUCCESS;
} /* libvirtGuestName_get */

/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestState
 * libvirtGuestState is subid 3 of libvirtGuestEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.4.1.12345.1.1.1.3
 * Description:
Current state of the active guest.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 4/8. Values:  running(1), blocked(2), paused(3), shutdown(4), shutoff(5), crashed(6)
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Extract the current value of the libvirtGuestState data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestState_val_ptr
 *        Pointer to storage for a long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
libvirtGuestState_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long * libvirtGuestState_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != libvirtGuestState_val_ptr );


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestState_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the libvirtGuestState data.
 * copy (* libvirtGuestState_val_ptr ) from rowreq_ctx->data
 */
    (* libvirtGuestState_val_ptr ) = rowreq_ctx->data.libvirtGuestState;

    return MFD_SUCCESS;
} /* libvirtGuestState_get */

/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestCpuCount
 * libvirtGuestCpuCount is subid 4 of libvirtGuestEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.12345.1.1.1.4
 * Description:
Number of virtual CPUs the virtual guest uses.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   0
 *   settable   0
 *
 * Ranges:  0 - 65535;
 *
 * Its syntax is UNSIGNED32 (based on perltype UNSIGNED32)
 * The net-snmp type is ASN_UNSIGNED. The C type decl is u_long (u_long)
 */
/**
 * Extract the current value of the libvirtGuestCpuCount data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestCpuCount_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
libvirtGuestCpuCount_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long * libvirtGuestCpuCount_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != libvirtGuestCpuCount_val_ptr );


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestCpuCount_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the libvirtGuestCpuCount data.
 * copy (* libvirtGuestCpuCount_val_ptr ) from rowreq_ctx->data
 */
    (* libvirtGuestCpuCount_val_ptr ) = rowreq_ctx->data.libvirtGuestCpuCount;

    return MFD_SUCCESS;
} /* libvirtGuestCpuCount_get */

/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestMemoryCurrent
 * libvirtGuestMemoryCurrent is subid 5 of libvirtGuestEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.12345.1.1.1.5
 * Description:
Current amount of memory (in MiB) used by the virtual guest.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   0
 *   settable   0
 *
 * Ranges:  0 - -1;
 *
 * Its syntax is GAUGE (based on perltype GAUGE)
 * The net-snmp type is ASN_GAUGE. The C type decl is u_long (u_long)
 */
/**
 * Extract the current value of the libvirtGuestMemoryCurrent data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestMemoryCurrent_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
libvirtGuestMemoryCurrent_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long * libvirtGuestMemoryCurrent_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != libvirtGuestMemoryCurrent_val_ptr );


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestMemoryCurrent_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the libvirtGuestMemoryCurrent data.
 * copy (* libvirtGuestMemoryCurrent_val_ptr ) from rowreq_ctx->data
 */
    (* libvirtGuestMemoryCurrent_val_ptr ) = rowreq_ctx->data.libvirtGuestMemoryCurrent;

    return MFD_SUCCESS;
} /* libvirtGuestMemoryCurrent_get */

/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestMemoryLimit
 * libvirtGuestMemoryLimit is subid 6 of libvirtGuestEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.12345.1.1.1.6
 * Description:
The maximum amount of memory (in MiB) that can be used by the virtual
	guest.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   0
 *   settable   0
 *
 * Ranges:  0 - -1;
 *
 * Its syntax is UNSIGNED32 (based on perltype UNSIGNED32)
 * The net-snmp type is ASN_UNSIGNED. The C type decl is u_long (u_long)
 */
/**
 * Extract the current value of the libvirtGuestMemoryLimit data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestMemoryLimit_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
libvirtGuestMemoryLimit_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long * libvirtGuestMemoryLimit_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != libvirtGuestMemoryLimit_val_ptr );


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestMemoryLimit_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the libvirtGuestMemoryLimit data.
 * copy (* libvirtGuestMemoryLimit_val_ptr ) from rowreq_ctx->data
 */
    (* libvirtGuestMemoryLimit_val_ptr ) = rowreq_ctx->data.libvirtGuestMemoryLimit;

    return MFD_SUCCESS;
} /* libvirtGuestMemoryLimit_get */

/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestCpuTime
 * libvirtGuestCpuTime is subid 7 of libvirtGuestEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.12345.1.1.1.7
 * Description:
The CPU time used by the virtual guest, in nanoseconds.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is COUNTER64 (based on perltype COUNTER64)
 * The net-snmp type is ASN_COUNTER64. The C type decl is U64 (U64)
 */
/**
 * Extract the current value of the libvirtGuestCpuTime data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestCpuTime_val_ptr
 *        Pointer to storage for a U64 variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
libvirtGuestCpuTime_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, U64 * libvirtGuestCpuTime_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != libvirtGuestCpuTime_val_ptr );

/*
 * TODO:231:o: |-> copy libvirtGuestCpuTime data.
 * get (* libvirtGuestCpuTime_val_ptr ).low and (* libvirtGuestCpuTime_val_ptr ).high from rowreq_ctx->data
 */
    (* libvirtGuestCpuTime_val_ptr ).high = rowreq_ctx->data.libvirtGuestCpuTime.high;
    (* libvirtGuestCpuTime_val_ptr ).low = rowreq_ctx->data.libvirtGuestCpuTime.low;


    return MFD_SUCCESS;
} /* libvirtGuestCpuTime_get */

/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestRowStatus
 * libvirtGuestRowStatus is subid 9 of libvirtGuestEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.4.1.12345.1.1.1.9
 * Description:
Status of the virtual guest.
	
	 A new virtual guest can be activated when either libvirtGuestName or
	 libvirtGuestUUID is specified and libvirtGuestState is either set to
	 running or paused.
	
	 A virtual guest can be destroyed by setting this column value to
	 'destroy'.
	
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 3/8. Values:  active(1), notInService(2), notReady(3), createAndGo(4), createAndWait(5), destroy(6)
 *
 * Its syntax is RowStatus (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Extract the current value of the libvirtGuestRowStatus data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestRowStatus_val_ptr
 *        Pointer to storage for a long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
libvirtGuestRowStatus_get( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long * libvirtGuestRowStatus_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != libvirtGuestRowStatus_val_ptr );

    (* libvirtGuestRowStatus_val_ptr ) = rowreq_ctx->data.libvirtGuestRowStatus;

    return MFD_SUCCESS;
} /* libvirtGuestRowStatus_get */



/** @} */
