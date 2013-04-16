CC=gcc

CFLAGS=-W -Werror -Wall -Wextra -std=c99 \
	-D_FORTIFY_SOURCE=2 -fstack-protector -g \
	-Wformat=2 -pedantic -pedantic-errors \
	-D_GNU_SOURCE=1 -D_BSD_SOURCE=1 \
	-I../../src

LDFLAGS=-pthread

cache: main.o cache.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o cache.o -o cache

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

cache.o: cache.c
	$(CC) $(CFLAGS) -c cache.c -o cache.o

clean:
	rm -f cache *.o
