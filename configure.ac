AC_INIT([libvirt-snmp],[0.0.1],[libvir-list@redhat.com],[],[http://libvirt.org])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_HEADERS([config.h])

AC_PREREQ([2.66])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strdup])
AC_CHECK_HEADERS([stdlib.h])
AC_FUNC_MALLOC
AC_TYPE_SIZE_T

dnl
dnl libvirt-snmp
dnl


dnl do we have libvirt installed?
LIBVIRT_CFLAGS=
LIBVIRT_LIBS=
LIBVIRT_FOUND=no
fail=0

AC_CHECK_HEADERS([libvirt/libvirt.h libvirt/virterror.h])
AC_CHECK_LIB([virt], [virConnectOpenAuth], [], [fail=1])

if test $fail = 1 ; then
    AC_MSG_ERROR([You must install libvirt in order to compile and run libvirt-snmp])
else
    LIBVIRT_LIBS="-lvirt"
fi

AC_SUBST([LIBVIRT_CFLAGS])
AC_SUBST([LIBVIRT_LIBS])

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
else
    AC_MSG_CHECKING([accessibility of $with_mibdir])
    if test ! -d "$with_mibdir" ; then
        AC_MSG_ERROR([$with_mibdir is not accessible])
    fi
    MIB_DIR=$with_mibdir
fi

AC_SUBST([MIB_DIR])

AC_OUTPUT(Makefile src/Makefile libvirt-snmp.spec)

