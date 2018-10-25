AC_INIT([libvirt-snmp],[0.0.4],[libvir-list@redhat.com],[],[https://libvirt.org])
AC_CONFIG_SRCDIR(src/libvirtSnmp.c)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror])

AM_SILENT_RULES([yes])

AC_PROG_CC
AM_PROG_CC_C_O

AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strdup])
AC_CHECK_HEADERS([stdlib.h])
AC_FUNC_MALLOC
AC_TYPE_SIZE_T
AC_C_CONST
AC_TYPE_SIGNAL

dnl
dnl libvirt-snmp
dnl


dnl do we have libvirt installed?
LIBVIRT_REQUIRED=1.0.6

PKG_CHECK_MODULES(LIBVIRT, libvirt >= $LIBVIRT_REQUIRED)
AC_SUBST(LIBVIRT_LIBS)
AC_SUBST(LIBVIRT_CFLAGS)

SNMP_CONFIG="net-snmp-config"
SNMP_CFLAGS=""
SNMP_LIBS=""
SNMP_FOUND="no"

AC_ARG_WITH([net-snmp], [AS_HELP_STRING([--with-netsnmp], [net-snmp library location@<:@default=check@:>@])])
if test "x$with_netsnmp" = "xno" ; then
    AC_MSG_ERROR([You must install net-snmp library in order to compile and run libvirt-snmp])
elif test "x$with_netsnmp" = "x" && test "x$PKG_CONFIG" != "x" ; then
    PKG_CHECK_MODULES(SNMP, net-snmp, [SNMP_FOUND=yes], [SNMP_FOUND=no])
fi
if test "$SNMP_FOUND" = "no" ; then
    if test "x$with_netsnmp" != "x" ; then
        SNMP_CONFIG="$with_netsnmp/bin/$SNMP_CONFIG"
    fi
    AC_MSG_CHECKING(net-snmp $SNMP_CONFIG)
    if ! $SNMP_CONFIG --version > /dev/null 2>&1 ; then
        AC_MSG_ERROR([Could not find $SNMP_CONFIG anywhere (see config.log for details)])
    fi
    SNMP_LIBS="`$SNMP_CONFIG --agent-libs`"
    SNMP_CFLAGS="`$SNMP_CONFIG --base-cflags`"
    AC_MSG_RESULT([yes])
fi

AC_SUBST([SNMP_CFLAGS])
AC_SUBST([SNMP_LIBS])

MIB_DIR=""

AC_ARG_WITH([mibdir], [AS_HELP_STRING([--with-mibdir], [MIB directory location@<:@default=check@:>@])])
if test "x$with_mibdir" = "xno" ; then
    AC_MSG_ERROR([You definitely want to specify MIB directory, so SNMP tools know libvirt MIB])
fi
if test "x$with_mibdir" = "x" ; then
    AC_MSG_CHECKING([MIB directories])
    MIB_DIR="`$SNMP_CONFIG --default-mibdirs`"
    if test -z "$MIB_DIR" ; then
        AC_MSG_ERROR([Could not find any default MIB directory. Please specify one by --with-mibdir])
    fi

    #choose the last one, it's likely system-wide
    MIB_DIR=$(echo $MIB_DIR | awk -F : '{print $NF}')

    AC_MSG_RESULT([found $MIB_DIR])

    dnl Replace net-snmp's exec_prefix with our own.
    dnl Note that ${exec_prefix} is kept verbatim at this point in time,
    dnl and will only be expanded later, when make is called: this makes
    dnl it possible to override such prefix at compilation or installation
    dnl time
    MIB_PREFIX="`$SNMP_CONFIG --prefix`"
    MIB_DIR='${exec_prefix}'"${MIB_DIR#$MIB_PREFIX}"
else
    AC_MSG_CHECKING([accessibility of $with_mibdir])
    if test ! -d "$with_mibdir" ; then
        AC_MSG_ERROR([$with_mibdir is not accessible])
    fi
    MIB_DIR=$with_mibdir
fi

AC_SUBST([MIB_DIR])

dnl pthread
PTHREAD_LIBS=
AC_CHECK_HEADERS(pthread.h, [], [AC_MSG_ERROR([pthread.h required])])
AC_CHECK_LIB(pthread, pthread_create, [PTHREAD_LIBS="-lpthread"])

AC_SUBST([PTHREAD_LIBS])

AC_OUTPUT(Makefile src/Makefile docs/Makefile libvirt-snmp.spec)

