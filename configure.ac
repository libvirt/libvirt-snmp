AC_INIT([libvirt-snmp],[0.0.2],[libvir-list@redhat.com],[],[http://libvirt.org])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_HEADERS([config.h])

AC_PREREQ([2.50])

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
LIBVIRT_REQUIRED=0.8.0

PKG_CHECK_MODULES(LIBVIRT, libvirt >= $LIBVIRT_REQUIRED)
LIBVIRT_LIBS="$LIBVIRT_LIBS -lvirt"
AC_SUBST(LIBVIRT_LIBS)

dnl do we have old libvirt?
save_CFLAGS="$CFLAGS"
save_LDFLAGS="$LDFLAGS"
save_LIBS="$LIBS"
CFLAGS="$CFLAGS $LIBVIRT_CFLAGS"
LDFLAGS="$CFLAGS $LIBVIRT_LIBS"
AC_CHECK_LIB([virt], [virEventRunDefaultImpl], [old=0], [old=1])
CFLAGS="$save_CFLAGS"
LDFLAGS="$save_LDFLAGS"
LIBS="$save_LIBS"
if test $old = 1 ; then
    AC_DEFINE_UNQUOTED([LIBVIRT_OLD], ["$old"], [we are using old libvirt
                        which does not have new event api])
    AC_CHECK_FUNCS([gettimeofday])
    AC_CHECK_FUNCS([memmove])
    AC_CHECK_HEADERS([fcntl.h])
    AC_CHECK_HEADERS([stddef.h])
    AC_CHECK_HEADERS([sys/time.h])
    AC_CHECK_TYPES([ptrdiff_t])
    AC_C_INLINE
    AC_FUNC_REALLOC
    AC_HEADER_STDBOOL
    AC_TYPE_PID_T
    AC_HEADER_TIME
fi
AM_CONDITIONAL([LIBVIRT_OLD], [test $old = 1])

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

dnl pthread
PTHREAD_LIBS=
AC_CHECK_HEADERS(pthread.h, [], [AC_MSG_ERROR([pthread.h required])])
AC_CHECK_LIB(pthread, pthread_create, [PTHREAD_LIBS="-lpthread"])

AC_SUBST([PTHREAD_LIBS])

AC_OUTPUT(Makefile src/Makefile docs/Makefile libvirt-snmp.spec)

