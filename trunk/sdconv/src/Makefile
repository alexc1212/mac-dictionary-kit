CC = g++

DESTDIR = ..
VER = 0.3
TOPDIR = ../..
RLSBINDIR = ${TOPDIR}/sdconv-current
RLSSRCDIR = ${TOPDIR}/sdconv-src
BINDIR = ${DESTDIR}/bin
LIBDIR = ${DESTDIR}/bin
INCLUDEDIR = ${DESTDIR}/include

CXXFLAGS = -Wall -I. -I$(INCLUDEDIR)/glib -I$(INCLUDEDIR) -isysroot /Developer/SDKs/MacOSX10.5.sdk -arch ppc -arch i386
GLIB_LDFLAGS = -lglib-2.0 -L$(LIBDIR)
PYTHON_LDFLAGS = -lpython
LDFLAGS = -dynamiclib $(GLIB_LDFLAGS) -lz -lintl -arch ppc -arch i386

$(BINDIR)/sdconv: sdconv.o $(LIBDIR)/libmdk.dylib
	$(CC) $(CXXFLAGS) $(GLIB_LDFLAGS) -Wall $^ -o $@

$(LIBDIR)/libmdk.dylib: index.o dict.o storage.o convert.o python.o
	$(CC) $(LDFLAGS) $(PYTHON_LDFLAGS) -install_name libmdk.dylib $^ -o $@

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
	rm -f $(BINDIR)/sdconv $(LIBDIR)/libmdk.dylib

