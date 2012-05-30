/*
 * libvirtGuestTable.h: SNMP table of running domains
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

#ifndef LIBVIRTGUESTTABLE_H
#define LIBVIRTGUESTTABLE_H

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup misc misc: Miscellaneous routines
 *
 * @{
 */
#include <net-snmp/library/asn1.h>

/* other required module components */
    /* *INDENT-OFF*  */
config_add_mib(LIBVIRT-MIB)
config_require(LIBVIRT-MIB/libvirtGuestTable/libvirtGuestTable_interface)
config_require(LIBVIRT-MIB/libvirtGuestTable/libvirtGuestTable_data_access)
config_require(LIBVIRT-MIB/libvirtGuestTable/libvirtGuestTable_data_get)
config_require(LIBVIRT-MIB/libvirtGuestTable/libvirtGuestTable_data_set)
    /* *INDENT-ON*  */

/* OID and column number definitions for libvirtGuestTable */
#include "libvirtGuestTable_oids.h"

/* enum definions */
#include "libvirtGuestTable_enums.h"

/* *********************************************************************
 * function declarations
 */
void init_libvirtGuestTable(void);
void shutdown_libvirtGuestTable(void);

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
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review libvirtGuestTable registration context.
     */
typedef netsnmp_data_list libvirtGuestTable_registration;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review libvirtGuestTable data context structure.
 * This structure is used to represent the data for libvirtGuestTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * libvirtGuestTable.
 */
typedef struct libvirtGuestTable_data_s {

        /*
         * libvirtGuestName(2)/OCTETSTR/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h
         */
   char   *libvirtGuestName;

        /*
         * libvirtGuestState(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   libvirtGuestState;

        /*
         * libvirtGuestCpuCount(4)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/R/d/h
         */
   u_long   libvirtGuestCpuCount;

        /*
         * libvirtGuestMemoryCurrent(5)/GAUGE/ASN_GAUGE/u_long(u_long)//l/A/w/e/R/d/h
         */
   u_long   libvirtGuestMemoryCurrent;

        /*
         * libvirtGuestMemoryLimit(6)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/R/d/h
         */
   u_long   libvirtGuestMemoryLimit;

        /*
         * libvirtGuestCpuTime(7)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h
         */
   U64   libvirtGuestCpuTime;

        /*
         * libvirtGuestRowStatus(9)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   libvirtGuestRowStatus;

} libvirtGuestTable_data;


/* *********************************************************************
 * TODO:115:o: |-> Review libvirtGuestTable undo context.
 * We're just going to use the same data structure for our
 * undo_context. If you want to do something more efficent,
 * define your typedef here.
 */
typedef libvirtGuestTable_data libvirtGuestTable_undo_data;

/*
 * TODO:120:r: |-> Review libvirtGuestTable mib index.
 * This structure is used to represent the index for libvirtGuestTable.
 */
typedef struct libvirtGuestTable_mib_index_s {

        /*
         * libvirtGuestUUID(1)/UUID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H
         */
   char   libvirtGuestUUID[16];
   size_t      libvirtGuestUUID_len;


} libvirtGuestTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review libvirtGuestTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     *
     * BE VERY CAREFUL TO TAKE INTO ACCOUNT THE MAXIMUM
     * POSSIBLE LENGHT FOR EVERY VARIABLE LENGTH INDEX!
     * Guessing 128 - col/entry(2)  - oid len(9)
*/
#define MAX_libvirtGuestTable_IDX_LEN     17


/* *********************************************************************
 * TODO:130:o: |-> Review libvirtGuestTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * libvirtGuestTable_rowreq_ctx pointer.
 */
typedef struct libvirtGuestTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_libvirtGuestTable_IDX_LEN];

    libvirtGuestTable_mib_index        tbl_idx;

    libvirtGuestTable_data              data;
    libvirtGuestTable_undo_data       * undo;
    unsigned int                column_set_flags; /* flags for set columns */


    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * TODO:131:o: |   |-> Add useful data to libvirtGuestTable rowreq context.
     */

    /*
     * storage for future expansion
     */
    netsnmp_data_list             *libvirtGuestTable_data_list;

} libvirtGuestTable_rowreq_ctx;

typedef struct libvirtGuestTable_ref_rowreq_ctx_s {
    libvirtGuestTable_rowreq_ctx *rowreq_ctx;
} libvirtGuestTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int libvirtGuestTable_pre_request(libvirtGuestTable_registration * user_context);
    int libvirtGuestTable_post_request(libvirtGuestTable_registration * user_context,
        int rc);

    int libvirtGuestTable_rowreq_ctx_init(libvirtGuestTable_rowreq_ctx *rowreq_ctx,
                                   void *user_init_ctx);
    void libvirtGuestTable_rowreq_ctx_cleanup(libvirtGuestTable_rowreq_ctx *rowreq_ctx);

    int libvirtGuestTable_commit(libvirtGuestTable_rowreq_ctx * rowreq_ctx);

    libvirtGuestTable_rowreq_ctx *
                  libvirtGuestTable_row_find_by_mib_index(libvirtGuestTable_mib_index *mib_idx);

extern const oid libvirtGuestTable_oid[];
extern const int libvirtGuestTable_oid_size;


#include "libvirtGuestTable_interface.h"
#include "libvirtGuestTable_data_access.h"
#include "libvirtGuestTable_data_get.h"
#include "libvirtGuestTable_data_set.h"

/*
 * DUMMY markers, ignore
 *
 * TODO:099:x: *************************************************************
 * TODO:199:x: *************************************************************
 * TODO:299:x: *************************************************************
 * TODO:399:x: *************************************************************
 * TODO:499:x: *************************************************************
 */

#ifdef __cplusplus
}
#endif

#endif /* LIBVIRTGUESTTABLE_H */
/** @} */
