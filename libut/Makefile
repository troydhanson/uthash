OBJS=libut.a
all: $(OBJS) 
INCDIR=./include
CFLAGS+=-I$(INCDIR)
CFLAGS+=-Wall -Wextra
CFLAGS+=-g

libut.a: libut.o utvector.o utmm.o ringbuf.o
	ar r $@ $^

libut.o: src/libut.c $(INCDIR)/libut.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

utvector.o: src/utvector.c $(INCDIR)/utvector.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

utmm.o: src/utmm.c $(INCDIR)/utmm.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

ringbuf.o: src/ringbuf.c $(INCDIR)/ringbuf.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

.PHONY: clean tests install

clean:
	rm -f $(OBJS) *.o
	make -C tests clean

tests: libut.a
	make -C tests 

install: libut.a
	cp $< /usr/local/lib
