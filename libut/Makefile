OBJS=libut.a
all: $(OBJS) 
INCDIR=./include
CFLAGS=-I$(INCDIR)
CFLAGS+=-Wall -Wextra
CFLAGS+=-g

libut.a: libut.o utvector.o utmm.o
	ar r $@ $^

libut.o: src/libut.c $(INCDIR)/libut.h
	$(CC) $(CFLAGS) -c $<

utvector.o: src/utvector.c $(INCDIR)/utvector.h
	$(CC) $(CFLAGS) -c $<

utmm.o: src/utmm.c $(INCDIR)/utmm.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean tests

clean:
	rm -f $(OBJS) *.o
	make -C tests clean

tests: libut.a
	make -C tests 
