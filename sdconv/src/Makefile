CC = g++

DESTDIR = ..
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
	rm *.o

dist-clean:
	rm $(BINDIR)/sdconv $(LIBDIR)/libmdk.dylib

