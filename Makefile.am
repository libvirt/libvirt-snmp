SUBDIRS=src/ docs/

EXTRA_DIST = \
	libvirt-snmp.spec libvirt-snmp.spec.in \
	autobuild.sh \
	autogen.sh \
	AUTHORS.in \
	$(NULL)

rpm: clean
		@(unset CDPATH ; $(MAKE) dist && rpmbuild -ta $(distdir).tar.gz)

dist-hook: gen-ChangeLog gen-AUTHORS

# Generate the ChangeLog file and insert it into the directory
# we're about to use to create a tarball.
.PHONY: gen-ChangeLog gen-AUTHORS
gen-ChangeLog:
	if test -x .git; then					\
	  $(top_srcdir)/build-aux/gitlog-to-changelog		\
	     > $(distdir)/cl-t;	\
	  rm -f $(distdir)/ChangeLog;				\
	  mv $(distdir)/cl-t $(distdir)/ChangeLog;		\
	fi

gen-AUTHORS:
	$(AM_V_GEN)if test -d $(srcdir)/.git; then                      \
	  out="`cd $(srcdir) && git log --pretty=format:'%aN <%aE>' | sort -u`" && \
	  perl -p -e "s/#authorslist#// and print '$$out'"            \
	    < $(srcdir)/AUTHORS.in > $(distdir)/AUTHORS-tmp &&        \
	  mv -f $(distdir)/AUTHORS-tmp $(distdir)/AUTHORS ;           \
	fi
