bin_PROGRAMS=libvirtMib_subagent

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

libvirtMib_subagent_CFLAGS=		\
	$(DRIVER_MODULE_CFLAGS)		\
	$(SNMP_CFLAGS)				\
	$(LIBVIRT_CFLAGS)			\
	$(COVERAGE_CFLAGS)			\
	$(PIE_CFLAGS)				\
	$(WARN_CFLAGS)				\
	$(LOCK_CHECKING_CFLAGS)

libvirtMib_subagent_LDFLAGS=	\
	$(PIE_LDFLAGS)		\
	$(COVERAGE_LDFLAGS)

libvirtMib_subagent_LDADD=		\
	$(PTHREAD_LIBS)				\
	$(LIBVIRT_LIBS)				\
	$(SNMP_LIBS)

EXTRA_DIST = LIBVIRT-MIB.txt

install-data-local:
	test -z "$(DESTDIR)$(MIB_DIR)" || @mkdir_p@ "$(DESTDIR)$(MIB_DIR)"
	$(INSTALL_DATA) "$(srcdir)/LIBVIRT-MIB.txt" \
		"$(DESTDIR)$(MIB_DIR)/LIBVIRT-MIB.txt"

uninstall-local:
	rm -f "$(DESTDIR)$(MIB_DIR)/LIBVIRT-MIB.txt"
	rmdir "$(DESTDIR)$(MIB_DIR)" || :

