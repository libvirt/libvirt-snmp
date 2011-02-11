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

autoreconf -i -f

cd $THEDIR

$srcdir/configure "$@" && {
    echo 
    echo "Now type 'make' to compile libvirt-snmp."
}
