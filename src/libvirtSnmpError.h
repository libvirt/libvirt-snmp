/*
 * libvirtSnmpError.h: libvirt error handling function
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

#ifndef __LIBVIRT_SNMP_ERROR_H__
# define __LIBVIRT_SNMP_ERROR_H__

# ifndef ATTRIBUTE_FMT_PRINTF
#  ifndef __clang__
#   define ATTRIBUTE_FMT_PRINTF(fmtpos, argpos) \
       __attribute__((__format__ (__gnu_printf__, fmtpos, argpos)))
#  else
#   define ATTRIBUTE_FMT_PRINTF(fmtpos, argpos) \
       __attribute__((__format__ (__printf__, fmtpos, argpos)))
#  endif
# endif

void printLibvirtError(const char *fmt, ...)
    ATTRIBUTE_FMT_PRINTF(1, 2);

#endif /* __LIBVIRT_SNMP_ERROR_H__ */
