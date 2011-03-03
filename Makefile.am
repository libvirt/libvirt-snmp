SUBDIRS=src/ docs/

EXTRA_DIST = \
	libvirt-snmp.spec libvirt-snmp.spec.in \
	autobuild.sh \
	autogen.sh

rpm: clean
		@(unset CDPATH ; $(MAKE) dist && rpmbuild -ta $(distdir).tar.gz)

gen-ChangeLog:
		if [ -d .git ]; then \
			git log --oneline | sed 's/[0-9a-f]* \(.*\)/\1/' > $(distdir)/ChangeLog; \
		else \
			echo "Prevent non-empty ChangeLog" > $(distdir)/ChangeLog ;\
		fi

.PHONY: gen-ChangeLog

dist-hook: gen-ChangeLog
