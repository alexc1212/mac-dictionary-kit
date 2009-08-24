CC = g++

DESTDIR = ..
VER = 0.4
TOPDIR = ../..
RLSBINDIR = ${TOPDIR}/sdconv-current
RLSSRCDIR = ${TOPDIR}/sdconv-src
BINDIR = ${DESTDIR}/bin
LIBDIR = ${DESTDIR}/bin
INCLUDEDIR = ${DESTDIR}/include

UBFLAGS = -isysroot /Developer/SDKs/MacOSX10.5.sdk -arch ppc -arch i386 -mmacosx-version-min=10.5
CXXFLAGS = -Wall -I. -I$(INCLUDEDIR)/glib -I$(INCLUDEDIR) $(UBFLAGS)
GLIB_LDFLAGS = -lglib-2.0 -L$(LIBDIR)
PYTHON_LDFLAGS = -lpython
LDFLAGS = $(GLIB_LDFLAGS) $(PYTHON_LDFLAGS) -lz $(UBFLAGS)

$(BINDIR)/sdconv: sdconv.o index.o dict.o storage.o convert.o python.o
	$(CC) $(LDFLAGS) $(GLIB_LDFLAGS) -Wall $^ -o $@

clean:
	rm -f *.o

release-bin: $(BINDIR)/sdconv
	rm -rf $(RLSBINDIR)
	mkdir -p $(RLSBINDIR)
	cp -r $(BINDIR) $(RLSBINDIR)/bin
	cp -r $(DESTDIR)/templates $(RLSBINDIR)/templates
	cp $(DESTDIR)/*.py $(RLSBINDIR)
	cp $(DESTDIR)/convert $(RLSBINDIR)
	@find $(RLSBINDIR) -type d -name .svn | xargs rm -rf

release-src:
	rm -f ${TOPDIR}/sdconv-$(VER).tar.bz2
	cd ${TOPDIR}; tar --exclude *.o --exclude .svn --exclude *.swp -cjf sdconv-$(VER).tar.bz2 sdconv

dist-clean:
	rm -f $(BINDIR)/sdconv

