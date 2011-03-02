POD2MAN = pod2man -c "Virtualization Support" -r "$(PACKAGE)-$(VERSION)"

EXTRA_DIST = \
	libvirtMib_subagent.pod

dist_man1_MANS = libvirtMib_subagent.1

libvirtMib_subagent.1: libvirtMib_subagent.pod
		$(AM_V_GEN)$(POD2MAN) $< $(srcdir)/$@

