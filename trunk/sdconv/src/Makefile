CC = g++

DESTDIR = ..
BINDIR = ${DESTDIR}/bin
LIBDIR = ${DESTDIR}/bin
INCLUDEDIR = ${DESTDIR}/include

CXXFLAGS = -Wall -I$(INCLUDEDIR)/glib -I$(INCLUDEDIR) -isysroot /Developer/SDKs/MacOSX10.5.sdk -arch ppc -arch i386
GLIBFLAGS = -lglib-2.0 -L$(LIBDIR)
LDFLAGS = -dynamiclib $(GLIBFLAGS) -lz -lintl -arch ppc -arch i386

$(BINDIR)/sdconv: sdconv.cpp $(LIBDIR)/libstardict.dylib
	$(CC) $(CXXFLAGS) $(GLIBFLAGS) -Wall $(LIBDIR)/libstardict.dylib $< -o $@

$(LIBDIR)/libstardict.dylib: index.o dict.o storage.o
	$(CC) $(LDFLAGS) -install_name libstardict.dylib $^ -o $@

clean:
	rm *.o

dist-clean:
	rm $(BINDIR)/sdconv $(LIBDIR)/libstardict.dylib

