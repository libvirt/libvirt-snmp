/*
 * libvirtGuestTable.c: SNMP table of running domains
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

#include <net-snmp/agent/mib_modules.h>

#include "libvirtGuestTable_interface.h"

const oid libvirtGuestTable_oid[] = { LIBVIRTGUESTTABLE_OID };
const int libvirtGuestTable_oid_size = OID_LENGTH(libvirtGuestTable_oid);

    libvirtGuestTable_registration  libvirtGuestTable_user_context;

void initialize_table_libvirtGuestTable(void);
void shutdown_table_libvirtGuestTable(void);


/**
 * Initializes the libvirtGuestTable module
 */
void
init_libvirtGuestTable(void)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:init_libvirtGuestTable","called\n"));

    /*
     * TODO:300:o: Perform libvirtGuestTable one-time module initialization.
     */

    /*
     * here we initialize all the tables we're planning on supporting
     */
    if (should_init("libvirtGuestTable"))
        initialize_table_libvirtGuestTable();

} /* init_libvirtGuestTable */

/**
 * Shut-down the libvirtGuestTable module (agent is exiting)
 */
void
shutdown_libvirtGuestTable(void)
{
    if (should_init("libvirtGuestTable"))
        shutdown_table_libvirtGuestTable();

}

/**
 * Initialize the table libvirtGuestTable
 *    (Define its contents and how it's structured)
 */
void
initialize_table_libvirtGuestTable(void)
{
    libvirtGuestTable_registration * user_context;
    u_long flags;

    DEBUGMSGTL(("verbose:libvirtGuestTable:initialize_table_libvirtGuestTable","called\n"));

    /*
     * TODO:301:o: Perform libvirtGuestTable one-time table initialization.
     */

    /*
     * TODO:302:o: |->Initialize libvirtGuestTable user context
     * if you'd like to pass in a pointer to some data for this
     * table, allocate or set it up here.
     */
    /*
     * a netsnmp_data_list is a simple way to store void pointers. A simple
     * string token is used to add, find or remove pointers.
     */
    user_context = netsnmp_create_data_list("libvirtGuestTable", NULL, NULL);

    /*
     * No support for any flags yet, but in the future you would
     * set any flags here.
     */
    flags = 0;

    /*
     * call interface initialization code
     */
    _libvirtGuestTable_initialize_interface(user_context, flags);
} /* initialize_table_libvirtGuestTable */

/**
 * Shutdown the table libvirtGuestTable
 */
void
shutdown_table_libvirtGuestTable(void)
{
    /*
     * call interface shutdown code
     */
    _libvirtGuestTable_shutdown_interface(&libvirtGuestTable_user_context);
}

/**
 * extra context initialization (eg default values)
 *
 * @param rowreq_ctx    : row request context
 * @param user_init_ctx : void pointer for user (parameter to rowreq_ctx_allocate)
 *
 * @retval MFD_SUCCESS  : no errors
 * @retval MFD_ERROR    : error (context allocate will fail)
 */
int
libvirtGuestTable_rowreq_ctx_init(libvirtGuestTable_rowreq_ctx *rowreq_ctx,
                           void *user_init_ctx)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_rowreq_ctx_init","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:210:o: |-> Perform extra libvirtGuestTable rowreq initialization. (eg DEFVALS)
     */

    return MFD_SUCCESS;
} /* libvirtGuestTable_rowreq_ctx_init */

/**
 * extra context cleanup
 *
 */
void libvirtGuestTable_rowreq_ctx_cleanup(libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_rowreq_ctx_cleanup","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    free(rowreq_ctx->data.libvirtGuestName);
} /* libvirtGuestTable_rowreq_ctx_cleanup */

/**
 * pre-request callback
 *
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_ERROR                : other error
 */
int
libvirtGuestTable_pre_request(libvirtGuestTable_registration * user_context)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_pre_request","called\n"));

    /*
     * TODO:510:o: Perform libvirtGuestTable pre-request actions.
     */

    return MFD_SUCCESS;
} /* libvirtGuestTable_pre_request */

/**
 * post-request callback
 *
 * Note:
 *   New rows have been inserted into the container, and
 *   deleted rows have been removed from the container and
 *   released.
 *
 * @param user_context
 * @param rc : MFD_SUCCESS if all requests succeeded
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : other error (ignored)
 */
int
libvirtGuestTable_post_request(libvirtGuestTable_registration * user_context, int rc)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_post_request","called\n"));

    /*
     * TODO:511:o: Perform libvirtGuestTable post-request actions.
     */

    /*
     * check to set if any rows were changed.
     */
    if (libvirtGuestTable_dirty_get()) {
        /*
         * check if request was successful. If so, this would be
         * a good place to save data to its persistent store.
         */
        if (MFD_SUCCESS == rc) {
            /*
             * save changed rows, if you haven't already
             */
        }

        libvirtGuestTable_dirty_set(0); /* clear table dirty flag */
    }

    return MFD_SUCCESS;
} /* libvirtGuestTable_post_request */


/** @{ */
