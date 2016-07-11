SOURCES = src/libut.c src/ringbuf.c src/utmm.c src/utvector.c
HEADERS = include/libut.h include/ringbuf.h include/utarray.h include/uthash.h include/utlist.h include/utmm.h include/utringbuffer.h include/utstring.h include/utvector.h
OBJS=$(SOURCES:.c=.o)
all: libut.a libut.so.2.0.1
INCDIR=./include
LIBDIR=/usr/lib
CFLAGS+=-I$(INCDIR)
CFLAGS+=-Wall -Wextra -Werror -fPIC
CFLAGS+=-g

libut.a: $(OBJS)
	ar r $@ $^

libut.so.2.0.1: $(OBJS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libut.so.2 $^ -o $@

.PHONY: clean tests install

clean:
	rm -f *.o *.a *.so*
	make -C tests clean

tests: libut.a
	make -C tests 

install: libut.a libut.so.2.0.1
	mkdir -p $(DESTDIR)$(LIBDIR)
	mkdir -p $(DESTDIR)$(LIBDIR)/pkgconfig
	mkdir -p $(DESTDIR)/usr/include
	cp libut.a $(DESTDIR)$(LIBDIR)
	cp libut.so.2.0.1 $(DESTDIR)$(LIBDIR)
	ln -sf $(LIBDIR)/libut.so.2.0.1 $(DESTDIR)$(LIBDIR)/libut.so.2
	ln -sf $(LIBDIR)/libut.so.2.0.1 $(DESTDIR)$(LIBDIR)/libut.so
	cp ./include/*.h $(DESTDIR)/usr/include
	sed -i -e "s|VERSION|2.0.1|" uthash.pc.in
	sed -i -e "s|LIBDIR|$(LIBDIR)|" uthash.pc.in
	cp uthash.pc.in $(DESTDIR)$(LIBDIR)/pkgconfig/uthash.pc
