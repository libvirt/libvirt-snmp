/*
 * libvirtGuestTable_interface.c: SNMP table interface
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

/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "libvirtGuestTable.h"


#include <net-snmp/agent/table_container.h>
#include <net-snmp/library/container.h>

#include "libvirtGuestTable_interface.h"

#include <ctype.h>

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
typedef struct libvirtGuestTable_interface_ctx_s {

   netsnmp_container              *container;
   netsnmp_cache                  *cache;

   libvirtGuestTable_registration *      user_ctx;

   netsnmp_table_registration_info  tbl_info;

   netsnmp_baby_steps_access_methods access_multiplexer;

    u_int                             table_dirty;

} libvirtGuestTable_interface_ctx;

static libvirtGuestTable_interface_ctx libvirtGuestTable_if_ctx;

static void _libvirtGuestTable_container_init(
    libvirtGuestTable_interface_ctx *if_ctx);
static void _libvirtGuestTable_container_shutdown(
    libvirtGuestTable_interface_ctx *if_ctx);


netsnmp_container *
libvirtGuestTable_container_get( void )
{
    return libvirtGuestTable_if_ctx.container;
}

libvirtGuestTable_registration *
libvirtGuestTable_registration_get( void )
{
    return libvirtGuestTable_if_ctx.user_ctx;
}

libvirtGuestTable_registration *
libvirtGuestTable_registration_set( libvirtGuestTable_registration * newreg )
{
    libvirtGuestTable_registration * old = libvirtGuestTable_if_ctx.user_ctx;
    libvirtGuestTable_if_ctx.user_ctx = newreg;
    return old;
}

int
libvirtGuestTable_container_size( void )
{
    return CONTAINER_SIZE(libvirtGuestTable_if_ctx.container);
}

u_int
libvirtGuestTable_dirty_get( void )
{
    return libvirtGuestTable_if_ctx.table_dirty;
}

void
libvirtGuestTable_dirty_set( u_int status )
{
    DEBUGMSGTL(("libvirtGuestTable:libvirtGuestTable_dirty_set",
                "called. was %d, now %d\n",
                libvirtGuestTable_if_ctx.table_dirty, status));
    libvirtGuestTable_if_ctx.table_dirty = status;
}

/*
 * mfd multiplexer modes
 */
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_pre_request;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_post_request;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_object_lookup;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_get_values;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_check_objects;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_undo_setup;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_set_values;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_undo_cleanup;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_undo_values;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_commit;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_undo_commit;
static Netsnmp_Node_Handler _mfd_libvirtGuestTable_irreversible_commit;

libvirtGuestTable_data *libvirtGuestTable_allocate_data(void);

/**
 * @internal
 * Initialize the table libvirtGuestTable
 *    (Define its contents and how it's structured)
 */
void
_libvirtGuestTable_initialize_interface(libvirtGuestTable_registration * reg_ptr,  u_long flags)
{
    netsnmp_baby_steps_access_methods *access_multiplexer =
        &libvirtGuestTable_if_ctx.access_multiplexer;
    netsnmp_table_registration_info *tbl_info = &libvirtGuestTable_if_ctx.tbl_info;
    netsnmp_handler_registration *reginfo;
    netsnmp_mib_handler *handler;
    int    mfd_modes = 0;

    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_initialize_interface","called\n"));


    /*************************************************
     *
     * save interface context for libvirtGuestTable
     */
    /*
     * Setting up the table's definition
     */
    netsnmp_table_helper_add_indexes(tbl_info,
    		ASN_PRIV_IMPLIED_OCTET_STR, /** index: libvirtGuestUUID */
                             0);

    /*  Define the minimum and maximum accessible columns.  This
        optimizes retrival. */
    tbl_info->min_column = LIBVIRTGUESTTABLE_MIN_COL;
    tbl_info->max_column = LIBVIRTGUESTTABLE_MAX_COL;

    /*
     * save users context
     */
    libvirtGuestTable_if_ctx.user_ctx = reg_ptr;

    /*
     * call data access initialization code
     */
    libvirtGuestTable_init_data(reg_ptr);

    /*
     * set up the container
     */
    _libvirtGuestTable_container_init(&libvirtGuestTable_if_ctx);
    if (NULL == libvirtGuestTable_if_ctx.container) {
        snmp_log(LOG_ERR,"could not initialize container for libvirtGuestTable\n");
        return;
    }

    /*
     * access_multiplexer: REQUIRED wrapper for get request handling
     */
    access_multiplexer->object_lookup = _mfd_libvirtGuestTable_object_lookup;
    access_multiplexer->get_values = _mfd_libvirtGuestTable_get_values;

    /*
     * no wrappers yet
     */
    access_multiplexer->pre_request = _mfd_libvirtGuestTable_pre_request;
    access_multiplexer->post_request = _mfd_libvirtGuestTable_post_request;


    /*
     * REQUIRED wrappers for set request handling
     */
    access_multiplexer->object_syntax_checks = _mfd_libvirtGuestTable_check_objects;
    access_multiplexer->undo_setup = _mfd_libvirtGuestTable_undo_setup;
    access_multiplexer->undo_cleanup = _mfd_libvirtGuestTable_undo_cleanup;
    access_multiplexer->set_values = _mfd_libvirtGuestTable_set_values;
    access_multiplexer->undo_sets = _mfd_libvirtGuestTable_undo_values;

    /*
     * no wrappers yet
     */
    access_multiplexer->commit = _mfd_libvirtGuestTable_commit;
    access_multiplexer->undo_commit = _mfd_libvirtGuestTable_undo_commit;
    access_multiplexer->irreversible_commit = _mfd_libvirtGuestTable_irreversible_commit;

    /*************************************************
     *
     * Create a registration, save our reg data, register table.
     */
    DEBUGMSGTL(("libvirtGuestTable:init_libvirtGuestTable",
                "Registering libvirtGuestTable as a mibs-for-dummies table.\n"));
    handler = netsnmp_baby_steps_access_multiplexer_get(access_multiplexer);
    reginfo = netsnmp_handler_registration_create("libvirtGuestTable", handler,
                                                  libvirtGuestTable_oid,
                                                  libvirtGuestTable_oid_size,
                                                  HANDLER_CAN_BABY_STEP |
                                                  HANDLER_CAN_RWRITE
                                                  );
    if(NULL == reginfo) {
        snmp_log(LOG_ERR,"error registering table libvirtGuestTable\n");
        return;
    }
    reginfo->my_reg_void = &libvirtGuestTable_if_ctx;

    /*************************************************
     *
     * set up baby steps handler, create it and inject it
     */
    if( access_multiplexer->object_lookup )
        mfd_modes |= BABY_STEP_OBJECT_LOOKUP;
    if( access_multiplexer->set_values )
        mfd_modes |= BABY_STEP_SET_VALUES;
    if( access_multiplexer->irreversible_commit )
        mfd_modes |= BABY_STEP_IRREVERSIBLE_COMMIT;
    if( access_multiplexer->object_syntax_checks )
        mfd_modes |= BABY_STEP_CHECK_OBJECT;

    if( access_multiplexer->pre_request )
        mfd_modes |= BABY_STEP_PRE_REQUEST;
    if( access_multiplexer->post_request )
        mfd_modes |= BABY_STEP_POST_REQUEST;

    if( access_multiplexer->undo_setup )
        mfd_modes |= BABY_STEP_UNDO_SETUP;
    if( access_multiplexer->undo_cleanup )
        mfd_modes |= BABY_STEP_UNDO_CLEANUP;
    if( access_multiplexer->undo_sets )
        mfd_modes |= BABY_STEP_UNDO_SETS;

    if( access_multiplexer->row_creation )
        mfd_modes |= BABY_STEP_ROW_CREATE;
    if( access_multiplexer->consistency_checks )
        mfd_modes |= BABY_STEP_CHECK_CONSISTENCY;
    if( access_multiplexer->commit )
        mfd_modes |= BABY_STEP_COMMIT;
    if( access_multiplexer->undo_commit )
        mfd_modes |= BABY_STEP_UNDO_COMMIT;

    handler = netsnmp_baby_steps_handler_get(mfd_modes);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject row_merge helper with prefix rootoid_len + 2 (entry.col)
     */
    handler = netsnmp_get_row_merge_handler(reginfo->rootoid_len + 2);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject container_table helper
     */
    handler =
        netsnmp_container_table_handler_get(tbl_info,
                                            libvirtGuestTable_if_ctx.container,
                                            TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    netsnmp_inject_handler( reginfo, handler );

    /*************************************************
     *
     * inject cache helper
     */
    if(NULL != libvirtGuestTable_if_ctx.cache) {
        handler = netsnmp_cache_handler_get(libvirtGuestTable_if_ctx.cache);
        netsnmp_inject_handler( reginfo, handler );
    }

    /*
     * register table
     */
    netsnmp_register_table(reginfo, tbl_info);

} /* _libvirtGuestTable_initialize_interface */

/**
 * @internal
 * Shutdown the table libvirtGuestTable
 */
void
_libvirtGuestTable_shutdown_interface(libvirtGuestTable_registration * reg_ptr)
{
    /*
     * shutdown the container
     */
    _libvirtGuestTable_container_shutdown(&libvirtGuestTable_if_ctx);
}

void
libvirtGuestTable_valid_columns_set(netsnmp_column_info *vc)
{
    libvirtGuestTable_if_ctx.tbl_info.valid_columns = vc;
} /* libvirtGuestTable_valid_columns_set */

/**
 * @internal
 * convert the index component stored in the context to an oid
 */
int
libvirtGuestTable_index_to_oid(netsnmp_index *oid_idx,
                         libvirtGuestTable_mib_index *mib_idx)
{
    int err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /*
     * libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H
     */
    netsnmp_variable_list var_libvirtGuestUUID;

    /*
     * set up varbinds
     */
    memset( &var_libvirtGuestUUID, 0x00, sizeof(var_libvirtGuestUUID) );
    var_libvirtGuestUUID.type = ASN_PRIV_IMPLIED_OCTET_STR;

    /*
     * chain temp index varbinds together
     */
    var_libvirtGuestUUID.next_variable =  NULL;


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_index_to_oid","called\n"));

        /* libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H */
    snmp_set_var_value(&var_libvirtGuestUUID, (u_char*)&mib_idx->libvirtGuestUUID,
                       mib_idx->libvirtGuestUUID_len * sizeof(mib_idx->libvirtGuestUUID[0]));


    err = build_oid_noalloc(oid_idx->oids, oid_idx->len, &oid_idx->len,
                           NULL, 0, &var_libvirtGuestUUID);
    if(err)
        snmp_log(LOG_ERR,"error %d converting index to oid\n", err);

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers( &var_libvirtGuestUUID );

    return err;
} /* libvirtGuestTable_index_to_oid */

/**
 * extract libvirtGuestTable indexes from a netsnmp_index
 *
 * @retval SNMP_ERR_NOERROR  : no error
 * @retval SNMP_ERR_GENERR   : error
 */
int
libvirtGuestTable_index_from_oid(netsnmp_index *oid_idx,
                         libvirtGuestTable_mib_index *mib_idx)
{
    int err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /*
     * libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H
     */
    netsnmp_variable_list var_libvirtGuestUUID;

    /*
     * set up varbinds
     */
    memset( &var_libvirtGuestUUID, 0x00, sizeof(var_libvirtGuestUUID) );
    var_libvirtGuestUUID.type = ASN_PRIV_IMPLIED_OCTET_STR;

    /*
     * chain temp index varbinds together
     */
    var_libvirtGuestUUID.next_variable =  NULL;


    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_index_from_oid","called\n"));

    /*
     * parse the oid into the individual index components
     */
    err = parse_oid_indexes( oid_idx->oids, oid_idx->len,
                             &var_libvirtGuestUUID );
    if (err == SNMP_ERR_NOERROR) {
        /*
         * copy out values
         */
    /*
     * NOTE: val_len is in bytes, libvirtGuestUUID_len might not be
     */
         if(var_libvirtGuestUUID.val_len > sizeof(mib_idx->libvirtGuestUUID))
             err = SNMP_ERR_GENERR;
         else {
             memcpy(mib_idx->libvirtGuestUUID, var_libvirtGuestUUID.val.string, var_libvirtGuestUUID.val_len);
             mib_idx->libvirtGuestUUID_len = var_libvirtGuestUUID.val_len / sizeof(mib_idx->libvirtGuestUUID[0]);
         }


    }

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers( &var_libvirtGuestUUID );

    return err;
} /* libvirtGuestTable_index_from_oid */


/*
 * libvirtGuestTable_allocate_data
 *
 * Purpose: create new libvirtGuestTable_data.
 */
libvirtGuestTable_data *
libvirtGuestTable_allocate_data(void)
{
    libvirtGuestTable_data *rtn = SNMP_MALLOC_TYPEDEF(libvirtGuestTable_data);

    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_allocate_data","called\n"));

    if(NULL == rtn) {
        snmp_log(LOG_ERR, "unable to malloc memory for new "
                 "libvirtGuestTable_data.\n");
    }

    return rtn;
} /* libvirtGuestTable_allocate_data */

/*
 * libvirtGuestTable_release_data
 *
 * Purpose: release libvirtGuestTable data.
 */
void
libvirtGuestTable_release_data(libvirtGuestTable_data *data)
{
    DEBUGMSGTL(("verbose:libvirtGuestTable:libvirtGuestTable_release_data","called\n"));

    free(data);
} /* libvirtGuestTable_release_data */

/* *********************************************************************
 * @internal
 * allocate resources for a libvirtGuestTable_rowreq_ctx
 */
libvirtGuestTable_rowreq_ctx *
libvirtGuestTable_allocate_rowreq_ctx(void *user_init_ctx)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  SNMP_MALLOC_TYPEDEF(libvirtGuestTable_rowreq_ctx);

    DEBUGMSGTL(("internal:libvirtGuestTable:libvirtGuestTable_allocate_rowreq_ctx","called\n"));

    if(NULL == rowreq_ctx) {
        snmp_log(LOG_ERR,"Couldn't allocate memory for a "
                 "libvirtGuestTable_rowreq_ctx.\n");
        return NULL;
    }

    rowreq_ctx->oid_idx.oids = rowreq_ctx->oid_tmp;

    rowreq_ctx->libvirtGuestTable_data_list = NULL;

    /*
     * if we allocated data, call init routine
     */
    if (!(rowreq_ctx->rowreq_flags & MFD_ROW_DATA_FROM_USER)) {
        if(SNMPERR_SUCCESS !=
            libvirtGuestTable_rowreq_ctx_init(rowreq_ctx, user_init_ctx)) {
           libvirtGuestTable_release_rowreq_ctx(rowreq_ctx);
           rowreq_ctx = NULL;
        }
    }

    return rowreq_ctx;
} /* libvirtGuestTable_allocate_rowreq_ctx */

/*
 * @internal
 * release resources for a libvirtGuestTable_rowreq_ctx
 */
void
libvirtGuestTable_release_rowreq_ctx(libvirtGuestTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("internal:libvirtGuestTable:libvirtGuestTable_release_rowreq_ctx","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    libvirtGuestTable_rowreq_ctx_cleanup(rowreq_ctx);

    if(rowreq_ctx->undo)
        libvirtGuestTable_release_data(rowreq_ctx->undo);

    /*
     * free index oid pointer
     */
    if(rowreq_ctx->oid_idx.oids != rowreq_ctx->oid_tmp)
        free(rowreq_ctx->oid_idx.oids);

    SNMP_FREE(rowreq_ctx);
} /* libvirtGuestTable_release_rowreq_ctx */

/**
 * @internal
 * wrapper
 */
static int
_mfd_libvirtGuestTable_pre_request(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *agtreq_info,
                            netsnmp_request_info *requests)
{
    int rc;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_pre_request",
                "called\n"));

    if (1 != netsnmp_row_merge_status_first(reginfo, agtreq_info)) {
        DEBUGMSGTL(("internal:libvirtGuestTable",
                    "skipping additional pre_request\n"));
        return SNMP_ERR_NOERROR;
    }

    rc = libvirtGuestTable_pre_request(libvirtGuestTable_if_ctx.user_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("libvirtGuestTable","error %d from "
                    "libvirtGuestTable_pre_request\n", rc));
        netsnmp_request_set_error_all(requests, SNMP_VALIDATE_ERR(rc));
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_pre_request */

/**
 * @internal
 * wrapper
 */
static int
_mfd_libvirtGuestTable_post_request(netsnmp_mib_handler *handler,
                             netsnmp_handler_registration *reginfo,
                             netsnmp_agent_request_info *agtreq_info,
                             netsnmp_request_info *requests)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    int rc, packet_rc;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_post_request",
                "called\n"));

    /*
     * release row context, if deleted
     */
    if (rowreq_ctx && (rowreq_ctx->rowreq_flags & MFD_ROW_DELETED))
        libvirtGuestTable_release_rowreq_ctx(rowreq_ctx);

    /*
     * wait for last call before calling user
     */
    if (1 != netsnmp_row_merge_status_last(reginfo, agtreq_info)) {
        DEBUGMSGTL(("internal:libvirtGuestTable",
                    "waiting for last post_request\n"));
        return SNMP_ERR_NOERROR;
    }

    packet_rc = netsnmp_check_all_requests_error(agtreq_info->asp, 0);
    if ((MFD_SUCCESS != packet_rc) && libvirtGuestTable_dirty_get()) {
        /*
         * we shouldn't get here. the undo steps should also clear
         * the dirty flags.
         */
        snmp_log(LOG_WARNING, "libvirtGuestTable dirty flag set in post_request "
                 "but status != SUCCESS.\n");
    }

    rc = libvirtGuestTable_post_request(libvirtGuestTable_if_ctx.user_ctx,packet_rc);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("libvirtGuestTable","error %d from "
                    "libvirtGuestTable_post_request\n", rc));
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_post_request */

NETSNMP_STATIC_INLINE int
_libvirtGuestTable_check_indexes(libvirtGuestTable_rowreq_ctx * rowreq_ctx)
{
    int                          rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_check_indexes","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);


    /* (INDEX) libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H */
    /* check defined range(s). */
    if( (SNMPERR_SUCCESS == rc)
       && (rowreq_ctx->tbl_idx.libvirtGuestUUID_len != 16)
     ) {
        rc = SNMP_ERR_WRONGLENGTH;
    }
       if (MFD_SUCCESS != rc)
          return rc;
        rc = libvirtGuestUUID_check_index( rowreq_ctx );
       if(MFD_SUCCESS != rc)
           return SNMP_ERR_NOCREATION;

    /*
     * if individual parts look ok, check them as a whole
     */
    return libvirtGuestTable_validate_index( libvirtGuestTable_if_ctx.user_ctx, rowreq_ctx );
} /* _libvirtGuestTable_check_indexes */
/**
 * @internal
 * wrapper
 */
static libvirtGuestTable_rowreq_ctx *
_mfd_libvirtGuestTable_rowreq_from_index(netsnmp_index *oid_idx, int * rc_ptr)
{
    libvirtGuestTable_rowreq_ctx * rowreq_ctx;
    libvirtGuestTable_mib_index mib_idx;
    int rc;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_rowreq_from_index",
                "called\n"));

    if (NULL == rc_ptr)
        rc_ptr = &rc;
    *rc_ptr = MFD_SUCCESS;

    memset(&mib_idx, 0x0, sizeof(mib_idx));

    /*
     * try to parse oid
     */
    *rc_ptr = libvirtGuestTable_index_from_oid(oid_idx, &mib_idx);
    if(MFD_SUCCESS != *rc_ptr) {
        DEBUGMSGT(("libvirtGuestTable", "error parsing index\n"));
        return NULL;
    }

    /*
     * allocate new context
     */
    rowreq_ctx = libvirtGuestTable_allocate_rowreq_ctx(NULL);
    if (NULL == rowreq_ctx) {
        *rc_ptr = MFD_ERROR;
        return NULL; /* msg already logged */
    }

    memcpy(&rowreq_ctx->tbl_idx, &mib_idx, sizeof(mib_idx));

    /*
     * check indexes
     */
    *rc_ptr = _libvirtGuestTable_check_indexes(rowreq_ctx);
    if(MFD_SUCCESS != *rc_ptr) {
        netsnmp_assert((*rc_ptr == SNMP_ERR_NOCREATION) ||
                       (*rc_ptr == SNMP_ERR_INCONSISTENTNAME));
        libvirtGuestTable_release_rowreq_ctx(rowreq_ctx);
        return NULL;
    }

    /*
     * copy indexes
     */
    rowreq_ctx->oid_idx.len = oid_idx->len;
    memcpy(rowreq_ctx->oid_idx.oids, oid_idx->oids, oid_idx->len * sizeof(oid));

    return rowreq_ctx;
} /* _mfd_libvirtGuestTable_rowreq_from_index */


/**
 * @internal
 * wrapper
 */
static int
_mfd_libvirtGuestTable_object_lookup(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    int                    rc = SNMP_ERR_NOERROR;
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_object_lookup","called\n"));

    /*
     * get our context from mfd
     * libvirtGuestTable_interface_ctx *if_ctx =
     *             (libvirtGuestTable_interface_ctx *)reginfo->my_reg_void;
     */

    if(NULL == rowreq_ctx) {
        netsnmp_table_request_info *tblreq_info;
        netsnmp_index oid_idx;

        tblreq_info = netsnmp_extract_table_info(requests);
        if(NULL == tblreq_info) {
            snmp_log(LOG_ERR, "request had no table info\n");
            return MFD_ERROR;
        }

        /*
         * try create rowreq
         */
        oid_idx.oids = tblreq_info->index_oid;
        oid_idx.len = tblreq_info->index_oid_len;

        rowreq_ctx = _mfd_libvirtGuestTable_rowreq_from_index(&oid_idx, &rc);
        if(MFD_SUCCESS == rc) {
            netsnmp_assert(NULL != rowreq_ctx);
            rowreq_ctx->rowreq_flags |= MFD_ROW_CREATED;
            /*
             * add rowreq_ctx to request data lists
             */
            netsnmp_container_table_row_insert(requests, (netsnmp_index*)rowreq_ctx);
        }
    }

    if (MFD_SUCCESS != rc)
        netsnmp_request_set_error_all(requests, rc);
    else
        libvirtGuestTable_row_prep(rowreq_ctx);

    return SNMP_VALIDATE_ERR(rc);
} /* _mfd_libvirtGuestTable_object_lookup */

/***********************************************************************
 *
 * GET processing
 *
 ***********************************************************************/
/*
 * @internal
 * Retrieve the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_libvirtGuestTable_get_column( libvirtGuestTable_rowreq_ctx *rowreq_ctx,
                       netsnmp_variable_list *var, int column )
{
    int rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_get_column",
                "called for %d\n", column));


    netsnmp_assert(NULL != rowreq_ctx);

    switch(column) {

    /* libvirtGuestName(2)/OCTETSTR/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h */
    case COLUMN_LIBVIRTGUESTNAME:
    var->type = ASN_OCTET_STR;
rc = libvirtGuestName_get(rowreq_ctx, (char **)&var->val.string, &var->val_len );
        break;

    /* libvirtGuestState(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTSTATE:
    var->val_len = sizeof(u_long);
    var->type = ASN_INTEGER;
rc = libvirtGuestState_get(rowreq_ctx, (u_long *)var->val.string );
        break;

    /* libvirtGuestCpuCount(4)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/R/d/h */
    case COLUMN_LIBVIRTGUESTCPUCOUNT:
    var->val_len = sizeof(u_long);
    var->type = ASN_UNSIGNED;
rc = libvirtGuestCpuCount_get(rowreq_ctx, (u_long *)var->val.string );
        break;

    /* libvirtGuestMemoryCurrent(5)/GAUGE/ASN_GAUGE/u_long(u_long)//l/A/w/e/R/d/h */
    case COLUMN_LIBVIRTGUESTMEMORYCURRENT:
    var->val_len = sizeof(u_long);
    var->type = ASN_GAUGE;
rc = libvirtGuestMemoryCurrent_get(rowreq_ctx, (u_long *)var->val.string );
        break;

    /* libvirtGuestMemoryLimit(6)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/R/d/h */
    case COLUMN_LIBVIRTGUESTMEMORYLIMIT:
    var->val_len = sizeof(u_long);
    var->type = ASN_UNSIGNED;
rc = libvirtGuestMemoryLimit_get(rowreq_ctx, (u_long *)var->val.string );
        break;

    /* libvirtGuestCpuTime(7)/COUNTER64/ASN_COUNTER64/struct counter64//l/A/w/e/r/d/h */
    case COLUMN_LIBVIRTGUESTCPUTIME:
    var->val_len = sizeof(struct counter64);
    var->type = ASN_COUNTER64;
rc = libvirtGuestCpuTime_get(rowreq_ctx, (struct counter64 *)var->val.string );
        break;

    /* libvirtGuestRowStatus(9)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTROWSTATUS:
    var->val_len = sizeof(u_long);
    var->type = ASN_INTEGER;
rc = libvirtGuestRowStatus_get(rowreq_ctx, (u_long *)var->val.string );
        break;

     default:
        if (LIBVIRTGUESTTABLE_MIN_COL <= column && column <= LIBVIRTGUESTTABLE_MAX_COL) {
            DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_get_column",
                "assume column %d is reserved\n", column));
            rc = MFD_SKIP;
        } else {
            snmp_log(LOG_ERR,
                "unknown column %d in _libvirtGuestTable_get_column\n", column);
        }
        break;
    }

    return rc;
} /* _libvirtGuestTable_get_column */

int
_mfd_libvirtGuestTable_get_values(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info * tri;
    u_char                     * old_string;
    void                      (*dataFreeHook)(void *);
    int                        rc;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_get_values","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    for(;requests; requests = requests->next) {
        /*
         * save old pointer, so we can free it if replaced
         */
        old_string = requests->requestvb->val.string;
        dataFreeHook = requests->requestvb->dataFreeHook;
        if(NULL == requests->requestvb->val.string) {
            requests->requestvb->val.string = requests->requestvb->buf;
            requests->requestvb->val_len = sizeof(requests->requestvb->buf);
        }
        else if(requests->requestvb->buf == requests->requestvb->val.string) {
            if(requests->requestvb->val_len != sizeof(requests->requestvb->buf))
                requests->requestvb->val_len = sizeof(requests->requestvb->buf);
        }

        /*
         * get column data
         */
        tri = netsnmp_extract_table_info(requests);
        if(NULL == tri)
            continue;

        rc = _libvirtGuestTable_get_column(rowreq_ctx, requests->requestvb, tri->colnum);
        if(rc) {
            if(MFD_SKIP == rc) {
                requests->requestvb->type = SNMP_NOSUCHINSTANCE;
                rc = SNMP_ERR_NOERROR;
            }
        }
        else if (NULL == requests->requestvb->val.string) {
            snmp_log(LOG_ERR,"NULL varbind data pointer!\n");
            rc = SNMP_ERR_GENERR;
        }
        if(rc)
            netsnmp_request_set_error(requests, SNMP_VALIDATE_ERR(rc));

        /*
         * if the buffer wasn't used previously for the old data (i.e. it
         * was allcoated memory)  and the get routine replaced the pointer,
         * we need to free the previous pointer.
         */
        if(old_string && (old_string != requests->requestvb->buf) &&
           (requests->requestvb->val.string != old_string)) {
            if(dataFreeHook)
                (*dataFreeHook)(old_string);
            else
                free(old_string);
        }
    } /* for results */

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_get_values */

/***********************************************************************
 *
 * SET processing
 *
 ***********************************************************************/

/*----------------------------------------------------------------------
 *
 * SET: Syntax checks
 *
 *---------------------------------------------------------------------*/
/*
 * @internal
 * Check the syntax for a particular column
 */
NETSNMP_STATIC_INLINE int
_libvirtGuestTable_check_column( libvirtGuestTable_rowreq_ctx *rowreq_ctx,
                         netsnmp_variable_list *var, int column )
{
    int rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_check_column",
                "called for %d\n", column));

    netsnmp_assert(NULL != rowreq_ctx);

    switch(column) {
    /* (INDEX) libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H */
    case COLUMN_LIBVIRTGUESTUUID:
        rc = SNMP_ERR_NOTWRITABLE; /* can not change index of active row */
        break;

    /* libvirtGuestName(2)/OCTETSTR/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h */
    case COLUMN_LIBVIRTGUESTNAME:
        rc = SNMP_ERR_NOTWRITABLE;
        break;

    /* libvirtGuestState(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTSTATE:
    rc = netsnmp_check_vb_type_and_size( var, ASN_INTEGER,
        sizeof( rowreq_ctx->data.libvirtGuestState ) );
    /* check that the value is one of defined enums */
    if( (SNMPERR_SUCCESS == rc)
 && ( *var->val.integer != LIBVIRTGUESTSTATE_RUNNING )
 && ( *var->val.integer != LIBVIRTGUESTSTATE_BLOCKED )
 && ( *var->val.integer != LIBVIRTGUESTSTATE_PAUSED )
 && ( *var->val.integer != LIBVIRTGUESTSTATE_SHUTDOWN )
 && ( *var->val.integer != LIBVIRTGUESTSTATE_SHUTOFF )
 && ( *var->val.integer != LIBVIRTGUESTSTATE_CRASHED )
        ) {
        rc = SNMP_ERR_WRONGVALUE;
        }
    if(SNMPERR_SUCCESS != rc) {
        DEBUGMSGTL(("libvirtGuestTable:_libvirtGuestTable_check_column:libvirtGuestState",
                    "varbind validation failed (eg bad type or size)\n"));
    }
    else {
        rc = libvirtGuestState_check_value( rowreq_ctx, *((u_long *)var->val.string) );
        if((MFD_SUCCESS != rc) && (MFD_NOT_VALID_EVER != rc) &&
           (MFD_NOT_VALID_NOW != rc)) {
            snmp_log(LOG_ERR, "bad rc %d from libvirtGuestState_check_value\n", rc);
            rc = SNMP_ERR_GENERR;
        }
    }
        break;

    /* libvirtGuestCpuCount(4)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/R/d/h */
    case COLUMN_LIBVIRTGUESTCPUCOUNT:
        rc = SNMP_ERR_NOTWRITABLE;
        break;

    /* libvirtGuestMemoryCurrent(5)/GAUGE/ASN_GAUGE/u_long(u_long)//l/A/w/e/R/d/h */
    case COLUMN_LIBVIRTGUESTMEMORYCURRENT:
        rc = SNMP_ERR_NOTWRITABLE;
        break;

    /* libvirtGuestMemoryLimit(6)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/R/d/h */
    case COLUMN_LIBVIRTGUESTMEMORYLIMIT:
        rc = SNMP_ERR_NOTWRITABLE;
        break;

    /* libvirtGuestCpuTime(7)/COUNTER64/ASN_COUNTER64/struct counter64//l/A/w/e/r/d/h */
    case COLUMN_LIBVIRTGUESTCPUTIME:
        rc = SNMP_ERR_NOTWRITABLE;
        break;

    /* libvirtGuestRowStatus(9)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTROWSTATUS:
rc = netsnmp_check_vb_rowstatus_value(var);
    if(SNMPERR_SUCCESS != rc) {
        DEBUGMSGTL(("libvirtGuestTable:_libvirtGuestTable_check_column:libvirtGuestRowStatus",
                    "varbind validation failed (eg bad type or size)\n"));
    }
    else {
        rc = libvirtGuestRowStatus_check_value( rowreq_ctx, *((u_long *)var->val.string) );
        if((MFD_SUCCESS != rc) && (MFD_NOT_VALID_EVER != rc) &&
           (MFD_NOT_VALID_NOW != rc)) {
            snmp_log(LOG_ERR, "bad rc %d from libvirtGuestRowStatus_check_value\n", rc);
            rc = SNMP_ERR_GENERR;
        }
    }
        break;

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column %d in _libvirtGuestTable_check_column\n", column);
    }

    return rc;
} /* _libvirtGuestTable_check_column */

int
_mfd_libvirtGuestTable_check_objects(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info * tri;
    int                          rc;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_check_objects","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    for(;requests; requests = requests->next) {

        /*
         * get column number from table request info, and check that column
         */
        tri = netsnmp_extract_table_info(requests);
        if(NULL == tri)
            continue;

        rc = _libvirtGuestTable_check_column(rowreq_ctx, requests->requestvb, tri->colnum);
        if(rc) {
            netsnmp_request_set_error(requests, SNMP_VALIDATE_ERR(rc));
            break;
        }

    } /* for results */

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_check_objects */


/*----------------------------------------------------------------------
 *
 * SET: Undo setup
 *
 *---------------------------------------------------------------------*/
/*
 * @internal
 * Set the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_libvirtGuestTable_undo_setup_column( libvirtGuestTable_rowreq_ctx *rowreq_ctx, int column )
{
    int rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_undo_setup_column",
                "called for %d\n", column));

    netsnmp_assert(NULL != rowreq_ctx);

    switch(column) {

    /* libvirtGuestState(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTSTATE:
        rowreq_ctx->column_set_flags |= COLUMN_LIBVIRTGUESTSTATE_FLAG;
        rc = libvirtGuestState_undo_setup(rowreq_ctx );
        break;

    /* libvirtGuestRowStatus(9)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTROWSTATUS:
        rowreq_ctx->column_set_flags |= COLUMN_LIBVIRTGUESTROWSTATUS_FLAG;
        rc = libvirtGuestRowStatus_undo_setup(rowreq_ctx );
        break;

     default:
         snmp_log(LOG_ERR,"unknown column %d in _libvirtGuestTable_undo_setup_column\n", column);
         break;
    }

    return rc;
} /* _libvirtGuestTable_undo_setup_column */


/**
 * @internal
 * undo setup
 */
int
_mfd_libvirtGuestTable_undo_setup(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    int                    rc;
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_undo_setup","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * allocate undo context
     */
    rowreq_ctx->undo = libvirtGuestTable_allocate_data();
    if(NULL == rowreq_ctx->undo) {
        /** msg already logged */
        netsnmp_request_set_error_all(requests, SNMP_ERR_RESOURCEUNAVAILABLE);
        return SNMP_ERR_NOERROR;
    }

    /*
     * row undo setup
     */
    rowreq_ctx->column_set_flags = 0;
    rc = libvirtGuestTable_undo_setup(rowreq_ctx);
    if (MFD_SUCCESS != rc)  {
        DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                    "libvirtGuestTable_undo_setup\n", rc));
        netsnmp_request_set_error_all(requests, SNMP_VALIDATE_ERR(rc));
    }
    else {
        /*
         * column undo setup
         */
        netsnmp_table_request_info * tri;
        for(;requests; requests = requests->next) {
            /*
             * set column data
             */
            tri = netsnmp_extract_table_info(requests);
            if(NULL == tri)
                continue;

            rc = _libvirtGuestTable_undo_setup_column(rowreq_ctx, tri->colnum);
            if(MFD_SUCCESS != rc)  {
                DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                            "libvirtGuestTable_undo_setup_column\n", rc));
                netsnmp_set_request_error(agtreq_info, requests, SNMP_VALIDATE_ERR(rc));
            }
        } /* for results */
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_undo_setup */

/**
 * @internal
 * undo setup
 */
int
_mfd_libvirtGuestTable_undo_cleanup(netsnmp_mib_handler *handler,
                             netsnmp_handler_registration *reginfo,
                             netsnmp_agent_request_info *agtreq_info,
                             netsnmp_request_info *requests)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    int rc;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_undo_cleanup","called\n"));

    /*
     * failed row create in early stages has no rowreq_ctx
     */
    if (NULL == rowreq_ctx)
        return MFD_SUCCESS;

    /*
     * call user cleanup
     */
    rc = libvirtGuestTable_undo_cleanup(rowreq_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                    "libvirtGuestTable_undo_cleanup\n", rc));
    }

    /*
     * release undo context, if needed
     */
    if(rowreq_ctx->undo) {
         libvirtGuestTable_release_data(rowreq_ctx->undo);
         rowreq_ctx->undo = NULL;
    }


    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_undo_cleanup */

/*----------------------------------------------------------------------
 *
 * SET: Set values
 *
 *---------------------------------------------------------------------*/
/*
 * @internal
 * Set the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_libvirtGuestTable_set_column( libvirtGuestTable_rowreq_ctx *rowreq_ctx,
                       netsnmp_variable_list *var, int column )
{
    int rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_set_column",
                "called for %d\n", column));

    netsnmp_assert(NULL != rowreq_ctx);

    switch(column) {

    /* libvirtGuestState(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTSTATE:
        rowreq_ctx->column_set_flags |= COLUMN_LIBVIRTGUESTSTATE_FLAG;
        rc = libvirtGuestState_set(rowreq_ctx, *((u_long *)var->val.string) );
        break;

    /* libvirtGuestRowStatus(9)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTROWSTATUS:
        rowreq_ctx->column_set_flags |= COLUMN_LIBVIRTGUESTROWSTATUS_FLAG;
        rc = libvirtGuestRowStatus_set(rowreq_ctx, *((u_long *)var->val.string) );
        break;

     default:
         snmp_log(LOG_ERR,"unknown column %d in _libvirtGuestTable_set_column\n", column);
         rc = SNMP_ERR_GENERR;
         break;
    }

    return rc;
} /* _libvirtGuestTable_set_column */

int
_mfd_libvirtGuestTable_set_values(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info * tri;
    int                          rc = SNMP_ERR_NOERROR;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_set_values","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    rowreq_ctx->column_set_flags = 0;
    for(;requests; requests = requests->next) {
        /*
         * set column data
         */
        tri = netsnmp_extract_table_info(requests);
        if(NULL == tri)
            continue;

        rc = _libvirtGuestTable_set_column(rowreq_ctx,
                                    requests->requestvb, tri->colnum);
        if(MFD_SUCCESS != rc)  {
            DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                        "libvirtGuestTable_set_column\n", rc));
            netsnmp_set_request_error(agtreq_info, requests, SNMP_VALIDATE_ERR(rc));
        }
    } /* for results */

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_set_values */

/*----------------------------------------------------------------------
 *
 * SET: commit
 *
 *---------------------------------------------------------------------*/
/**
 * @internal
 * commit the values
 */
int
_mfd_libvirtGuestTable_commit(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    int                    rc;
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_commit","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    rc = libvirtGuestTable_commit(rowreq_ctx);
    if (MFD_SUCCESS != rc) {
        DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                    "libvirtGuestTable_commit\n", rc));
        netsnmp_request_set_error_all(requests, SNMP_VALIDATE_ERR(rc));
    }

    if (rowreq_ctx->rowreq_flags & MFD_ROW_DIRTY) {
        /*
         * if we successfully commited this row, set the dirty flag. Use the
         * current value + 1 (i.e. dirty = # rows changed).
         * this is checked in post_request...
         */
        libvirtGuestTable_dirty_set( libvirtGuestTable_dirty_get() + 1 ); /* set table dirty flag */
    }

    return SNMP_ERR_NOERROR;
}

int
_mfd_libvirtGuestTable_undo_commit(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    int                    rc;
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_undo_commit","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    if (rowreq_ctx->rowreq_flags & MFD_ROW_DIRTY) {
        u_int  d = libvirtGuestTable_dirty_get();

        netsnmp_assert(d != 0);
        if(d)
            libvirtGuestTable_dirty_set( d - 1 );
    }

    rc = libvirtGuestTable_undo_commit(rowreq_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                    "libvirtGuestTable_undo_commit\n", rc));
    }

    if (rowreq_ctx->rowreq_flags & MFD_ROW_DIRTY) {
        snmp_log(LOG_WARNING, "libvirtGuestTable row dirty flag still set after undo_commit\n");
        rowreq_ctx->rowreq_flags &= ~MFD_ROW_DIRTY;
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_commit */

/*----------------------------------------------------------------------
 *
 * SET: Undo
 *
 *---------------------------------------------------------------------*/
/**
 * @internal
 * undo the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_libvirtGuestTable_undo_column( libvirtGuestTable_rowreq_ctx *rowreq_ctx,
                       netsnmp_variable_list *var, int column )
{
    int rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_undo_column",
                "called for %d\n", column));

    netsnmp_assert(NULL != rowreq_ctx);

    switch(column) {

    /* libvirtGuestState(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTSTATE:
        rc = libvirtGuestState_undo(rowreq_ctx);
        break;

    /* libvirtGuestRowStatus(9)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h */
    case COLUMN_LIBVIRTGUESTROWSTATUS:
        rc = libvirtGuestRowStatus_undo(rowreq_ctx);
        break;

     default:
         snmp_log(LOG_ERR,"unknown column %d in _libvirtGuestTable_undo_column\n", column);
         break;
    }

    return rc;
} /* _libvirtGuestTable_undo_column */

int
_mfd_libvirtGuestTable_undo_values(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    int                    rc;
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info * tri;

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_undo_values","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    rc = libvirtGuestTable_undo(rowreq_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                    "libvirtGuestTable_undo\n", rc));
    }

    for(;requests; requests = requests->next) {
        /*
         * set column data
         */
        tri = netsnmp_extract_table_info(requests);
        if(NULL == tri)
            continue;

        rc = _libvirtGuestTable_undo_column(rowreq_ctx, requests->requestvb,
                                     tri->colnum);
        if (MFD_SUCCESS != rc) {
            /*
             * nothing we can do about it but log it
             */
            DEBUGMSGTL(("libvirtGuestTable:mfd","error %d from "
                        "libvirtGuestTable_undo_column\n", rc));
        }
    } /* for results */

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_undo_values */

/*----------------------------------------------------------------------
 *
 * SET: irreversible commit
 *
 *---------------------------------------------------------------------*/
/**
 * @internal
 * commit irreversible actions
 */
int
_mfd_libvirtGuestTable_irreversible_commit(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    libvirtGuestTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:libvirtGuestTable:_mfd_libvirtGuestTable_irreversible:commit","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * check for and handle row creation/deletion
     * and update column exist flags...
     */
    if (rowreq_ctx->rowreq_flags & MFD_ROW_DELETED) {
        if (! (rowreq_ctx->rowreq_flags & MFD_ROW_CREATED))
            CONTAINER_REMOVE(libvirtGuestTable_if_ctx.container, rowreq_ctx);
    }
    else {
        if (rowreq_ctx->column_set_flags) {
           rowreq_ctx->column_set_flags = 0;
        }
        if (rowreq_ctx->rowreq_flags & MFD_ROW_CREATED) {
            rowreq_ctx->rowreq_flags &= ~MFD_ROW_CREATED;
            CONTAINER_INSERT(libvirtGuestTable_if_ctx.container, rowreq_ctx);
        }
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_libvirtGuestTable_irreversible_commit */

/***********************************************************************
 *
 * DATA ACCESS
 *
 ***********************************************************************/
static void _container_free(netsnmp_container *container);

/**
 * @internal
 */
static int
_cache_load(netsnmp_cache *cache, void *vmagic)
{
    DEBUGMSGTL(("internal:libvirtGuestTable:_cache_load","called\n"));

    if((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR, "invalid cache for libvirtGuestTable_cache_load\n");
        return -1;
    }

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * call user code
     */
    return libvirtGuestTable_container_load((netsnmp_container*)cache->magic);
} /* _cache_load */

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache *cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:libvirtGuestTable:_cache_free","called\n"));

    if((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR, "invalid cache in libvirtGuestTable_cache_free\n");
        return;
    }

    container = (netsnmp_container*)cache->magic;

    _container_free(container);
} /* _cache_free */

/**
 * @internal
 */
static void
_container_item_free(libvirtGuestTable_rowreq_ctx *rowreq_ctx, void *context)
{
    DEBUGMSGTL(("internal:libvirtGuestTable:_container_item_free","called\n"));

    if(NULL == rowreq_ctx)
        return;

    libvirtGuestTable_release_rowreq_ctx(rowreq_ctx);
} /* _container_item_free */

/**
 * @internal
 */
static void
_container_free(netsnmp_container *container)
{
    DEBUGMSGTL(("internal:libvirtGuestTable:_container_free","called\n"));

    if (NULL == container) {
        snmp_log(LOG_ERR, "invalid container in libvirtGuestTable_container_free\n");
        return;
    }

    /*
     * call user code
     */
    libvirtGuestTable_container_free(container);

    /*
     * free all items. inefficient, but easy.
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *)_container_item_free,
                    NULL);
} /* _container_free */

/**
 * @internal
 * initialize the container with functions or wrappers
 */
void
_libvirtGuestTable_container_init(libvirtGuestTable_interface_ctx *if_ctx)
{
    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_container_init","called\n"));

    /*
     * cache init
     */
    if_ctx->cache = netsnmp_cache_create(30, /* timeout in seconds */
                                         _cache_load, _cache_free,
                                         libvirtGuestTable_oid,
                                         libvirtGuestTable_oid_size);

    if(NULL == if_ctx->cache) {
        snmp_log(LOG_ERR, "error creating cache for libvirtGuestTable\n");
        return;
    }

    if_ctx->cache->flags = NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;

    libvirtGuestTable_container_init(&if_ctx->container, if_ctx->cache);
    if(NULL == if_ctx->container)
        if_ctx->container = netsnmp_container_find("libvirtGuestTable:table_container");
    if(NULL == if_ctx->container) {
        snmp_log(LOG_ERR,"error creating container in "
                 "libvirtGuestTable_container_init\n");
        return;
    }

    if (NULL != if_ctx->cache)
        if_ctx->cache->magic = (void*)if_ctx->container;
} /* _libvirtGuestTable_container_init */

/**
 * @internal
 * shutdown the container with functions or wrappers
 */
void
_libvirtGuestTable_container_shutdown(libvirtGuestTable_interface_ctx *if_ctx)
{
    DEBUGMSGTL(("internal:libvirtGuestTable:_libvirtGuestTable_container_shutdown","called\n"));

    libvirtGuestTable_container_shutdown(if_ctx->container);

    _container_free(if_ctx->container);

} /* _libvirtGuestTable_container_shutdown */


libvirtGuestTable_rowreq_ctx *
libvirtGuestTable_row_find_by_mib_index(libvirtGuestTable_mib_index *mib_idx)
{
    libvirtGuestTable_rowreq_ctx   *rowreq_ctx;
    oid                      oid_tmp[MAX_OID_LEN];
    netsnmp_index            oid_idx;
    int                      rc;

    /*
     * set up storage for OID
     */
    oid_idx.oids = oid_tmp;
    oid_idx.len = sizeof(oid_tmp)/sizeof(oid);

    /*
     * convert
     */
    rc = libvirtGuestTable_index_to_oid(&oid_idx, mib_idx);
    if (MFD_SUCCESS != rc)
        return NULL;

    rowreq_ctx = CONTAINER_FIND(libvirtGuestTable_if_ctx.container, &oid_idx);

    return rowreq_ctx;
}

