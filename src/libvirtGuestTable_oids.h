/*
 * libvirtGuestTable_oids.h: SNMP table OIDs
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

#ifndef LIBVIRTGUESTTABLE_OIDS_H
#define LIBVIRTGUESTTABLE_OIDS_H

#ifdef __cplusplus
extern "C" {
#endif


/* column number definitions for table libvirtGuestTable */
#define LIBVIRTGUESTTABLE_OID              1,3,6,1,4,1,12345,1,1


#define COLUMN_LIBVIRTGUESTUUID         1
    
#define COLUMN_LIBVIRTGUESTNAME         2
    
#define COLUMN_LIBVIRTGUESTSTATE         3
#define COLUMN_LIBVIRTGUESTSTATE_FLAG    (0x1 << 0)
    
#define COLUMN_LIBVIRTGUESTCPUCOUNT         4
    
#define COLUMN_LIBVIRTGUESTMEMORYCURRENT         5
    
#define COLUMN_LIBVIRTGUESTMEMORYLIMIT         6
    
#define COLUMN_LIBVIRTGUESTCPUTIME         7
    
#define COLUMN_LIBVIRTGUESTROWSTATUS         9
#define COLUMN_LIBVIRTGUESTROWSTATUS_FLAG    (0x1 << 1)
    

#define LIBVIRTGUESTTABLE_MIN_COL   COLUMN_LIBVIRTGUESTNAME
#define LIBVIRTGUESTTABLE_MAX_COL   COLUMN_LIBVIRTGUESTROWSTATUS
    

    /*
     * TODO:405:r: Review LIBVIRTGUESTTABLE_SETTABLE_COLS macro.
     * OR together all the writable cols.
     */
#define LIBVIRTGUESTTABLE_SETTABLE_COLS (COLUMN_LIBVIRTGUESTSTATE_FLAG | COLUMN_LIBVIRTGUESTROWSTATUS_FLAG)
    /*
     * TODO:405:r: Review LIBVIRTGUESTTABLE_REQUIRED_COLS macro.
     * OR together all the required rows for row creation.
     * default is writable cols w/out defaults.
     */
#define LIBVIRTGUESTTABLE_REQUIRED_COLS (COLUMN_LIBVIRTGUESTSTATE_FLAG | COLUMN_LIBVIRTGUESTROWSTATUS_FLAG)
    

#ifdef __cplusplus
}
#endif

#endif /* LIBVIRTGUESTTABLE_OIDS_H */
