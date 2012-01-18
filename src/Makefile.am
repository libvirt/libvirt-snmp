bin_PROGRAMS=libvirtMib_subagent

AM_CFLAGS =	\
		$(COVERAGE_CFLAGS) \
		$(DRIVER_MODULE_CFLAGS) \
		$(SNMP_CFLAGS)          \
		$(LIBVIRT_CFLAGS) \
		$(WARN_CFLAGS)          \
		$(LOCK_CHECKING_CFLAGS)

AM_LDFLAGS = 	\
		$(COVERAGE_LDFLAGS) \
		$(PTHREAD_LIBS) \
		$(LIBVIRT_LIBS) \
		$(SNMP_LIBS)

LIBVIRT_OLD_SRCS = \
		threads.c \
		event_poll.c \
		memory.c \
		util.c

LIBVIRT_OLD_HDRS = \
		internal.h \
		ignore-value.h \
		threads.h \
		event_poll.h \
		memory.h \
		util.h

USER_SRCS = 	\
		libvirtGuestTable_data_get.c \
		libvirtGuestTable_data_set.c \
		libvirtGuestTable_data_access.c \
		libvirtSnmpError.c \
		libvirtNotifications.c \
		libvirtSnmp.c

USER_HDRS = 	\
		libvirtGuestTable_data_get.h \
		libvirtGuestTable_data_set.h \
		libvirtGuestTable_data_access.h \
		libvirtSnmpError.h \
		libvirtNotifications.h \
		libvirtSnmp.h

SRCS = 		\
		${USER_SRCS} \
		libvirtGuestTable.c \
		libvirtMib_subagent.c \
		libvirtGuestTable_interface.c

HDRS = \
		${USER_HDRS} \
		libvirtGuestTable_enums.h \
		libvirtGuestTable_oids.h \
		libvirtGuestTable_interface.h \
		libvirtGuestTable.h

libvirtMib_subagent_SOURCES=${SRCS} ${HDRS}
libvirtMib_subagent_LDFLAGS=${AM_LDFLAGS}

if LIBVIRT_OLD
libvirtMib_subagent_SOURCES+=${LIBVIRT_OLD_SRCS} ${LIBVIRT_OLD_HDRS}
endif

EXTRA_DIST = LIBVIRT-MIB.txt

install-data-local:
	test -z "$(DESTDIR)$(MIB_DIR)" || @mkdir_p@ "$(DESTDIR)$(MIB_DIR)"
	$(INSTALL_DATA) "$(srcdir)/LIBVIRT-MIB.txt" \
		"$(DESTDIR)$(MIB_DIR)/LIBVIRT-MIB.txt"

uninstall-local:
	rm -f "$(DESTDIR)$(MIB_DIR)/LIBVIRT-MIB.txt"
	rmdir "$(DESTDIR)$(MIB_DIR)" || :

