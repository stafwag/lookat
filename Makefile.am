bin_PROGRAMS = lookat bekijk
man_MANS = lookat.1 bekijk.1
lookat_SOURCES = get_dir.c get_dir.h menu.c menu.h configfile.c configfile.h win.h win.c view.c view.h input.c input.h msg.c msg.h eng.h xmalloc.c xmalloc.h outofmemory.c outofmemory.h getline.c getline.h lookat_configfile.c lookat_configfile.h xstring.c xstring.h color.c color.h eng.h common.h ncurses_h.h
lookat_LDADD   = lookat.o
docdir         = ${datadir}/doc/lookat/
doc_DATA       = README LEESMIJ 
ENG_DOCFILE    = $(docdir)README
NED_DOCFILE    = $(docdir)LEESMIJ
CONFIGFILE=$(sysconfdir)/lookat.conf
lookat.o: bekijk.c
	$(COMPILE) -D_ENG -DCONFIGFILE=\"$(CONFIGFILE)\" -DDOCFILE=\"$(ENG_DOCFILE)\" -o lookat.o -c bekijk.c 

bekijk_SOURCES = bekijk.c get_dir.c get_dir.h menu.c menu.h configfile.c configfile.h win.h win.c view.c view.h input.c input.h msg.c msg.h ned.h xmalloc.c xmalloc.h outofmemory.c outofmemory.h getline.c getline.h lookat_configfile.h lookat_configfile.c xstring.c xstring.h color.c color.h ned.h common.h ncurses_h.h
bekijk.o: bekijk.c
	$(COMPILE) -D_NED -DCONFIGFILE=\"$(CONFIGFILE)\" -DDOCFILE=\"$(NED_DOCFILE)\" -o bekijk.o -c bekijk.c

sysconf_DATA=lookat.conf.default

EXTRA_DIST = $(man_MANS) ${sysconf_DATA} LEESMIJ examples/blue_config.cfg

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)examples
	$(INSTALL_DATA) $(srcdir)/examples/blue_config.cfg $(DESTDIR)$(docdir)examples/blue_config.cfg

	@if test -f $(DESTDIR)$(CONFIGFILE); then \
		echo ;\
		echo "$@ will not overwrite the existing $(DESTDIR)$(CONFIGFILE)" ; \
		echo ;\
	else \
		echo ;\
		echo "Installing new configuration file $(DESTDIR)$(CONFIGFILE)"; \
		echo ;\
		$(INSTALL_DATA) lookat.conf.default $(DESTDIR)$(CONFIGFILE); \
	fi


