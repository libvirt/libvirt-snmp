#!/bin/sh
# Run this to generate all the initial makefiles, etc.

set -e
srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

THEDIR=`pwd`
cd $srcdir

if test -z "$*"; then
	echo "I am going to run ./configure with not arguments - if you "
        echo "wish to pass any extra arguments to it, please specify them on "
        echo "the $0 command line."
fi

# Automake requires that ChangeLog exist.
# Real ChangeLog/AUTHORS is auto-generated from GIT logs at
# make dist time, but automake requires that it
# exists at all times :-(
touch ChangeLog AUTHORS

mkdir -p build-aux
autoreconf -if

cd $THEDIR

$srcdir/configure "$@" && {
    echo
    echo "Now type 'make' to compile libvirt-snmp."
}
