/*
 * libvirtGuestTable_data_access.c: SNMP table data access routines
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


#include "libvirtGuestTable_data_access.h"

#include "libvirtSnmp.h"

/** @ingroup interface
 * @addtogroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
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

/**
 * initialization for libvirtGuestTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param libvirtGuestTable_reg
 *        Pointer to libvirtGuestTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
libvirtGuestTable_init_data(libvirtGuestTable_registration * libvirtGuestTable_reg)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize libvirtGuestTable data.
     */
    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    /*
     * if you are the sole writer for the file, you could
     * open it here. However, as stated earlier, we are assuming
     * the worst case, which in this case means that the file is
     * written to by someone else, and might not even exist when
     * we start up. So we can't do anything here.
     */
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    return MFD_SUCCESS;
} /* libvirtGuestTable_init_data */

/**
 * container overview
 *
 */

/**
 * container initialization
 *
 * @param container_ptr_ptr A pointer to a container pointer. If you
 *        create a custom container, use this parameter to return it
 *        to the MFD helper. If set to NULL, the MFD helper will
 *        allocate a container for you.
 * @param  cache A pointer to a cache structure. You can set the timeout
 *         and other cache flags using this pointer.
 *
 *  This function is called at startup to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases. If no custom
 *  container is allocated, the MFD code will create one for your.
 *
 *  This is also the place to set up cache behavior. The default, to
 *  simply set the cache timeout, will work well with the default
 *  container. If you are using a custom container, you may want to
 *  look at the cache helper documentation to see if there are any
 *  flags you want to set.
 *
 * @remark
 *  This would also be a good place to do any initialization needed
 *  for you data source. For example, opening a connection to another
 *  process that will supply the data, opening a database, etc.
 */
void
libvirtGuestTable_container_init(netsnmp_container **container_ptr_ptr,
                             netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_container_init","called\n"));

    if (NULL == container_ptr_ptr) {
        snmp_log(LOG_ERR,"bad container param to libvirtGuestTable_container_init\n");
        return;
    }

    if (libvirtSnmpInit()) {
        snmp_log(LOG_ERR,"cannot initialize libvirt\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    if (NULL == cache) {
        snmp_log(LOG_ERR,"bad cache param to libvirtGuestTable_container_init\n");
        return;
    }

    /*
     * TODO:345:A: Set up libvirtGuestTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = LIBVIRTGUESTTABLE_CACHE_TIMEOUT; /* seconds */
} /* libvirtGuestTable_container_init */

/**
 * container shutdown
 *
 * @param container_ptr A pointer to the container.
 *
 *  This function is called at shutdown to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases.
 *
 *  This function is called before libvirtGuestTable_container_free().
 *
 * @remark
 *  This would also be a good place to do any cleanup needed
 *  for you data source. For example, closing a connection to another
 *  process that supplied the data, closing a database, etc.
 */
void
libvirtGuestTable_container_shutdown(netsnmp_container *container_ptr)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_container_shutdown","called\n"));

    if (NULL == container_ptr) {
        snmp_log(LOG_ERR,"bad params to libvirtGuestTable_container_shutdown\n");
        return;
    }

    libvirtSnmpShutdown();
} /* libvirtGuestTable_container_shutdown */

/**
 * load initial data
 *
 * TODO:350:M: Implement libvirtGuestTable data load
 * This function will also be called by the cache helper to load
 * the container again (after the container free function has been
 * called to free the previous contents).
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  libvirtGuestTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
libvirtGuestTable_container_load(netsnmp_container *container)
{
    if (libvirtSnmpLoadGuests(container))
    	return MFD_ERROR;
	return MFD_SUCCESS;
} /* libvirtGuestTable_container_load */

/**
 * container clean up
 *
 * @param container container with all current items
 *
 *  This optional callback is called prior to all
 *  item's being removed from the container. If you
 *  need to do any processing before that, do it here.
 *
 * @note
 *  The MFD helper will take care of releasing all the row contexts.
 *
 */
void
libvirtGuestTable_container_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_container_free","called\n"));

    /*
     * TODO:380:M: Free libvirtGuestTable container data.
     */
} /* libvirtGuestTable_container_free */

/**
 * prepare row for processing.
 *
 *  When the agent has located the row for a request, this function is
 *  called to prepare the row for processing. If you fully populated
 *  the data context during the index setup phase, you may not need to
 *  do anything.
 *
 * @param rowreq_ctx pointer to a context.
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 */
int
libvirtGuestTable_row_prep( libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* libvirtGuestTable_row_prep */

/*
 * TODO:420:r: Implement libvirtGuestTable index validation.
 */
/*---------------------------------------------------------------------
 * LIBVIRT-MIB::libvirtGuestEntry.libvirtGuestUUID
 * libvirtGuestUUID is subid 1 of libvirtGuestEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.4.1.12345.1.1.1.1
 * Description:
The UUID of the virtual guest.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 4x-2x-2x-2x-6x
 *
 * Ranges:  16;
 *
 * Its syntax is UUID (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 16)
 *
 *
 *
 * NOTE: NODE libvirtGuestUUID IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of libvirtGuestUUID index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       libvirtGuestTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The length is in (one of) the range set(s):  16
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
libvirtGuestUUID_check_index( libvirtGuestTable_rowreq_ctx *rowreq_ctx )
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestUUID_check_index","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check libvirtGuestTable index libvirtGuestUUID.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.libvirtGuestUUID)
     */

    return MFD_SUCCESS; /* libvirtGuestUUID index ok */
} /* libvirtGuestUUID_check_index */

/**
 * verify specified index is valid.
 *
 * This check is independent of whether or not the values specified for
 * the columns of the new row are valid. Column values and row consistency
 * will be checked later. At this point, only the index values should be
 * checked.
 *
 * All of the individual index validation functions have been called, so this
 * is the place to make sure they are valid as a whole when combined. If
 * you only have one index, then you probably don't need to do anything else
 * here.
 *
 * @note Keep in mind that if the indexes refer to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 *
 * @param libvirtGuestTable_reg
 *        Pointer to the user registration data
 * @param libvirtGuestTable_rowreq_ctx
 *        Pointer to the users context.
 * @retval MFD_SUCCESS            : success
 * @retval MFD_CANNOT_CREATE_NOW  : index not valid right now
 * @retval MFD_CANNOT_CREATE_EVER : index never valid
 */
int
libvirtGuestTable_validate_index( libvirtGuestTable_registration * libvirtGuestTable_reg,
                           libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;
    int ret;

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_validate_index","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:430:M: |-> Validate potential libvirtGuestTable index.
     */
    /* Check that the UUID points to real domain */
    ret = libvirtSnmpCheckDomainExists((unsigned char*) rowreq_ctx->tbl_idx.libvirtGuestUUID);
    if(ret) {
        snmp_log(LOG_WARNING,"invalid index for a new row in the "
                 "libvirtGuestTable table.\n");
        /*
         * determine failure type.
         *
         * If the index could not ever be created, return MFD_NOT_EVER
         * If the index can not be created under the present circumstances
         * (even though it could be created under other circumstances),
         * return MFD_NOT_NOW.
         */
        /*
         * For now, let's assume the UUID is valid and the machine could be
         * created in the future, so _NOW error is appropriate.
         */
        if(0) {
            return MFD_CANNOT_CREATE_EVER;
        }
        else {
            return MFD_CANNOT_CREATE_NOW;
        }
    }

    return rc;
} /* libvirtGuestTable_validate_index */

/** @} */
