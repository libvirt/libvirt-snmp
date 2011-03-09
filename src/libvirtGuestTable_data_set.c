/*
 * libvirtGuestTable_data_set.c: SNMP table data set routines
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
#include "libvirtSnmp.h"

/** @defgroup data_set data_set: Routines to set data
 *
 * These routines are used to set the value for individual objects. The
 * row context is passed, along with the new value.
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
    /*
     * NOTE: if you update this chart, please update the versions in
     *       local/mib2c-conf.d/parent-set.m2i
     *       agent/mibgroup/helpers/baby_steps.c
     * while you're at it.
     */
    /*
     ***********************************************************************
     * Baby Steps Flow Chart (2004.06.05)                                  *
     *                                                                     *
     * +--------------+    +================+    U = unconditional path    *
     * |optional state|    ||required state||    S = path for success      *
     * +--------------+    +================+    E = path for error        *
     ***********************************************************************
     *
     *                        +--------------+
     *                        |     pre      |
     *                        |   request    |
     *                        +--------------+
     *                               | U
     * +-------------+        +==============+
     * |    row    |f|<-------||  object    ||
     * |  create   |1|      E ||  lookup    ||
     * +-------------+        +==============+
     *     E |   | S                 | S
     *       |   +------------------>|
     *       |                +==============+
     *       |              E ||   check    ||
     *       |<---------------||   values   ||
     *       |                +==============+
     *       |                       | S
     *       |                +==============+
     *       |       +<-------||   undo     ||
     *       |       |      E ||   setup    ||
     *       |       |        +==============+
     *       |       |               | S
     *       |       |        +==============+
     *       |       |        ||    set     ||-------------------------->+
     *       |       |        ||   value    || E                         |
     *       |       |        +==============+                           |
     *       |       |               | S                                 |
     *       |       |        +--------------+                           |
     *       |       |        |    check     |-------------------------->|
     *       |       |        |  consistency | E                         |
     *       |       |        +--------------+                           |
     *       |       |               | S                                 |
     *       |       |        +==============+         +==============+  |
     *       |       |        ||   commit   ||-------->||     undo   ||  |
     *       |       |        ||            || E       ||    commit  ||  |
     *       |       |        +==============+         +==============+  |
     *       |       |               | S                     U |<--------+
     *       |       |        +--------------+         +==============+
     *       |       |        | irreversible |         ||    undo    ||
     *       |       |        |    commit    |         ||     set    ||
     *       |       |        +--------------+         +==============+
     *       |       |               | U                     U |
     *       |       +-------------->|<------------------------+
     *       |                +==============+
     *       |                ||   undo     ||
     *       |                ||  cleanup   ||
     *       |                +==============+
     *       +---------------------->| U
     *                               |
     *                          (err && f1)------------------->+
     *                               |                         |
     *                        +--------------+         +--------------+
     *                        |    post      |<--------|      row     |
     *                        |   request    |       U |    release   |
     *                        +--------------+         +--------------+
     *
     */

/**
 * Setup up context with information needed to undo a set request.
 *
 * This function will be called before the individual node undo setup
 * functions are called. If you need to do any undo setup that is not
 * related to a specific column, you can do it here.
 *
 * Note that the undo context has been allocated with
 * libvirtGuestTable_allocate_data(), but may need extra
 * initialization similar to what you may have done in
 * libvirtGuestTable_rowreq_ctx_init().
 * Note that an individual node's undo_setup function will only be called
 * if that node is being set to a new value.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in the node's undo_setup
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (libvirtGuestTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
libvirtGuestTable_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_undo_setup","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:451:M: |-> Setup libvirtGuestTable undo.
     * set up libvirtGuestTable undo information, in preparation for a set.
     * Undo storage is in (* libvirtGuestRowStatus_val_ptr )*
     */

    return rc;
} /* libvirtGuestTable_undo_setup */

/**
 * Undo a set request.
 *
 * This function will be called before the individual node undo
 * functions are called. If you need to do any undo that is not
 * related to a specific column, you can do it here.
 *
 * Note that an individual node's undo function will only be called
 * if that node is being set to a new value.
 *
 * If there is anything  specific to a particular column (e.g. releasing
 * memory for a string), you should do that setup in the node's undo
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (libvirtGuestTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
libvirtGuestTable_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_undo","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:451:M: |-> libvirtGuestTable undo.
     * libvirtGuestTable undo information, in response to a failed set.
     * Undo storage is in (* libvirtGuestRowStatus_val_ptr )*
     */

    return rc;
} /* libvirtGuestTable_undo_setup */

/**
 * Cleanup up context undo information.
 *
 * This function will be called after set/commit processing. If you
 * allocated any resources in undo_setup, this is the place to release
 * those resources.
 *
 * This function is called regardless of the success or failure of the set
 * request. If you need to perform different steps for cleanup depending
 * on success or failure, you can add a flag to the rowreq_ctx.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (libvirtGuestTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
libvirtGuestTable_undo_cleanup( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_undo_cleanup","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:452:M: |-> Cleanup libvirtGuestTable undo.
     * Undo storage is in (* libvirtGuestRowStatus_val_ptr )*
     */

    return rc;
} /* libvirtGuestTable_undo_cleanup */

/**
 * commit new values.
 *
 * At this point, you should have done everything you can to ensure that
 * this commit will not fail.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * libvirtGuestTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param libvirtGuestTable_rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
libvirtGuestTable_commit( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;
    int             save_flags;

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_commit","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * save flags, then clear until we actually do something
     */
    save_flags = rowreq_ctx->column_set_flags;
    rowreq_ctx->column_set_flags = 0;

    if (save_flags & COLUMN_LIBVIRTGUESTROWSTATUS_FLAG) {
       save_flags &= ~COLUMN_LIBVIRTGUESTROWSTATUS_FLAG; /* clear libvirtGuestRowStatus */
       /*
        * TODO:482:o: |-> commit column libvirtGuestRowStatus.
        */
       switch(rowreq_ctx->data.libvirtGuestRowStatus) {
       case ROWSTATUS_DESTROY:
    	   rc = libvirtSnmpDestroy((unsigned char*) rowreq_ctx->tbl_idx.libvirtGuestUUID);
    	   break;
       case ROWSTATUS_CREATEANDGO:
    	   if (save_flags & COLUMN_LIBVIRTGUESTSTATE_FLAG) {
    		   rc = libvirtSnmpCreate((unsigned char*) rowreq_ctx->tbl_idx.libvirtGuestUUID,
    				   rowreq_ctx->data.libvirtGuestState);
    		   /* don't process the state column later in this function */
    		   save_flags &= ~COLUMN_LIBVIRTGUESTSTATE_FLAG;
    	   } else {
    		   rc = libvirtSnmpCreate((unsigned char*) rowreq_ctx->tbl_idx.libvirtGuestUUID,
    				   LIBVIRTGUESTSTATE_RUNNING);
    	   }
    	   break;
       case ROWSTATUS_CREATEANDWAIT:
       case ROWSTATUS_NOTINSERVICE:
       case ROWSTATUS_NOTREADY:
       case ROWSTATUS_ACTIVE:
       default:
    	   rc = -1;
       }
       if (rc) {
           snmp_log(LOG_ERR,"libvirtGuestTable column libvirtGuestRowStatus commit failed\n");
           rc = MFD_ERROR;
       }
       else {
            /*
             * set flag, in case we need to undo libvirtGuestRowStatus
             */
            rowreq_ctx->column_set_flags |= COLUMN_LIBVIRTGUESTROWSTATUS_FLAG;
       }
    }

    /*
     * commit libvirtGuestTable data
     * 1) check the column's flag in save_flags to see if it was set.
     * 2) clear the flag when you handle that column
     * 3) set the column's flag in column_set_flags if it needs undo
     *    processing in case of a failure.
     */
    if (save_flags & COLUMN_LIBVIRTGUESTSTATE_FLAG) {
       save_flags &= ~COLUMN_LIBVIRTGUESTSTATE_FLAG; /* clear libvirtGuestState */
       /*
        * TODO:482:o: |-> commit column libvirtGuestState.
        */
   	   rc = libvirtSnmpChangeState((unsigned char*)rowreq_ctx->tbl_idx.libvirtGuestUUID,
   			   rowreq_ctx->data.libvirtGuestState,
   			   rowreq_ctx->undo->libvirtGuestState);
       if(rc) {
           snmp_log(LOG_ERR,"libvirtGuestTable column libvirtGuestState commit failed\n");
           rc = MFD_ERROR;
       }
       else {
            /*
             * set flag, in case we need to undo libvirtGuestState
             */
            rowreq_ctx->column_set_flags |= COLUMN_LIBVIRTGUESTSTATE_FLAG;
       }
    }

    /*
     * if we successfully commited this row, set the dirty flag.
     */
    if (MFD_SUCCESS == rc) {
        rowreq_ctx->rowreq_flags |= MFD_ROW_DIRTY;
    }

    if (save_flags) {
       snmp_log(LOG_ERR, "unhandled columns (0x%x) in commit\n", save_flags);
       return MFD_ERROR;
    }

    return rc;
} /* libvirtGuestTable_commit */

/**
 * undo commit new values.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * libvirtGuestTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param libvirtGuestTable_rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
libvirtGuestTable_undo_commit( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_undo_commit","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:485:M: |-> Undo libvirtGuestTable commit.
     * check the column's flag in rowreq_ctx->column_set_flags to see
     * if it was set during commit, then undo it.
     *
     * eg: if (rowreq_ctx->column_set_flags & COLUMN__FLAG) {}
     */

    
    /*
     * if we successfully un-commited this row, clear the dirty flag.
     */
    if (MFD_SUCCESS == rc) {
        rowreq_ctx->rowreq_flags &= ~MFD_ROW_DIRTY;
    }

    return rc;
} /* libvirtGuestTable_undo_commit */

/*
 * TODO:440:M: Implement libvirtGuestTable node value checks.
 * TODO:450:M: Implement libvirtGuestTable undo functions.
 * TODO:460:M: Implement libvirtGuestTable set functions.
 * TODO:480:M: Implement libvirtGuestTable commit functions.
 */
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
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestState_val
 *        A long containing the new value.
 *
 * @retval MFD_SUCCESS        : incoming value is legal
 * @retval MFD_NOT_VALID_NOW  : incoming value is not valid now
 * @retval MFD_NOT_VALID_EVER : incoming value is never valid
 *
 * This is the place to check for requirements that are not
 * expressed in the mib syntax (for example, a requirement that
 * is detailed in the description for an object).
 *
 * You should check that the requested change between the undo value and the
 * new value is legal (ie, the transistion from one value to another
 * is legal).
 *      
 *@note
 * This check is only to determine if the new value
 * is \b potentially valid. This is the first check of many, and
 * is one of the simplest ones.
 * 
 *@note
 * this is not the place to do any checks for values
 * which depend on some other value in the mib. Those
 * types of checks should be done in the
 * libvirtGuestTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *    The value is one of  running(1), blocked(2), paused(3), shutdown(4), shutoff(5), crashed(6)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
libvirtGuestState_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestState_val)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestState_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid libvirtGuestState value.
     */
    /*
     * The only allowed transitions:
     * running -> paused
     * running -> shutdown
     * paused -> running
     */
    if (rowreq_ctx->data.libvirtGuestState == LIBVIRTGUESTSTATE_RUNNING
    		&& (libvirtGuestState_val == LIBVIRTGUESTSTATE_PAUSED
    				|| libvirtGuestState_val == LIBVIRTGUESTSTATE_SHUTDOWN))
    	return MFD_SUCCESS;

    if (rowreq_ctx->data.libvirtGuestState == LIBVIRTGUESTSTATE_PAUSED
    		&& libvirtGuestState_val == LIBVIRTGUESTSTATE_RUNNING)
    	return MFD_SUCCESS;

    return MFD_NOT_VALID_NOW; /* libvirtGuestState value illegal */
} /* libvirtGuestState_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (libvirtGuestTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * libvirtGuestTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
libvirtGuestState_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestState_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup libvirtGuestState undo.
     */
    /*
     * copy libvirtGuestState data
     * set rowreq_ctx->undo->libvirtGuestState from rowreq_ctx->data.libvirtGuestState
     */
    rowreq_ctx->undo->libvirtGuestState = rowreq_ctx->data.libvirtGuestState;


    return MFD_SUCCESS;
} /* libvirtGuestState_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param libvirtGuestState_val
 *        A long containing the new value.
 */
int
libvirtGuestState_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestState_val )
{

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestState_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:461:M: |-> Set libvirtGuestState value.
     * set libvirtGuestState value in rowreq_ctx->data
     */
    rowreq_ctx->data.libvirtGuestState = libvirtGuestState_val;

    return MFD_SUCCESS;
} /* libvirtGuestState_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
libvirtGuestState_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestState_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up libvirtGuestState undo.
     */
    /*
     * copy libvirtGuestState data
     * set rowreq_ctx->data.libvirtGuestState from rowreq_ctx->undo->libvirtGuestState
     */
    rowreq_ctx->data.libvirtGuestState = rowreq_ctx->undo->libvirtGuestState;

    
    return MFD_SUCCESS;
} /* libvirtGuestState_undo */

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
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param libvirtGuestRowStatus_val
 *        A long containing the new value.
 *
 * @retval MFD_SUCCESS        : incoming value is legal
 * @retval MFD_NOT_VALID_NOW  : incoming value is not valid now
 * @retval MFD_NOT_VALID_EVER : incoming value is never valid
 *
 * This is the place to check for requirements that are not
 * expressed in the mib syntax (for example, a requirement that
 * is detailed in the description for an object).
 *
 * You should check that the requested change between the undo value and the
 * new value is legal (ie, the transistion from one value to another
 * is legal).
 *      
 *@note
 * This check is only to determine if the new value
 * is \b potentially valid. This is the first check of many, and
 * is one of the simplest ones.
 * 
 *@note
 * this is not the place to do any checks for values
 * which depend on some other value in the mib. Those
 * types of checks should be done in the
 * libvirtGuestTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *    The value is one of  active(1), notInService(2), notReady(3), createAndGo(4), createAndWait(5), destroy(6)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
libvirtGuestRowStatus_check_value( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestRowStatus_val)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestRowStatus_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid libvirtGuestRowStatus value.
     */
    if (libvirtGuestRowStatus_val == ROWSTATUS_NOTREADY
    		|| libvirtGuestRowStatus_val == ROWSTATUS_CREATEANDWAIT)
    	return MFD_NOT_VALID_EVER;

    return MFD_SUCCESS; /* libvirtGuestRowStatus value not illegal */
} /* libvirtGuestRowStatus_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (libvirtGuestTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * libvirtGuestTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
libvirtGuestRowStatus_undo_setup( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestRowStatus_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup libvirtGuestRowStatus undo.
     */
    /*
     * copy libvirtGuestRowStatus data
     * set rowreq_ctx->undo->libvirtGuestRowStatus from rowreq_ctx->data.libvirtGuestRowStatus
     */
    rowreq_ctx->undo->libvirtGuestRowStatus = rowreq_ctx->data.libvirtGuestRowStatus;


    return MFD_SUCCESS;
} /* libvirtGuestRowStatus_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param libvirtGuestRowStatus_val
 *        A long containing the new value.
 */
int
libvirtGuestRowStatus_set( libvirtGuestTable_rowreq_ctx *rowreq_ctx, u_long libvirtGuestRowStatus_val )
{

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestRowStatus_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:461:M: |-> Set libvirtGuestRowStatus value.
     * set libvirtGuestRowStatus value in rowreq_ctx->data
     */
    rowreq_ctx->data.libvirtGuestRowStatus = libvirtGuestRowStatus_val;

    return MFD_SUCCESS;
} /* libvirtGuestRowStatus_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
libvirtGuestRowStatus_undo( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestRowStatus_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up libvirtGuestRowStatus undo.
     */
    /*
     * copy libvirtGuestRowStatus data
     * set rowreq_ctx->data.libvirtGuestRowStatus from rowreq_ctx->undo->libvirtGuestRowStatus
     */
    rowreq_ctx->data.libvirtGuestRowStatus = rowreq_ctx->undo->libvirtGuestRowStatus;

    
    return MFD_SUCCESS;
} /* libvirtGuestRowStatus_undo */

/** @} */
