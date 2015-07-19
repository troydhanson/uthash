OBJS=libut.a
all: $(OBJS) 
INCDIR=../include
CFLAGS=-I$(INCDIR)
CFLAGS+=-Wall -Wextra
CFLAGS+=-g

libut.a: libut.o utvector.o utmm.o
	ar r $@ $^

libut.o: libut.c $(INCDIR)/libut.h
	$(CC) $(CFLAGS) -c $<

utvector.o: utvector.c $(INCDIR)/utvector.h
	$(CC) $(CFLAGS) -c $<

utmm.o: utmm.c $(INCDIR)/utmm.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean 

clean:
	rm -f $(OBJS) *.o
