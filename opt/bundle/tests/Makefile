PROGS=test1 test2 test3 test4 test5 test6 test7
OBJS=$(patsubst %,%.o,$(PROGS))

CFLAGS = -I../include -L../src
CFLAGS += -g
CFLAGS += -Wall 
LDFLAGS=-lut

TEST_TARGET=run_tests
TESTS=./do_tests

all: $(OBJS) $(PROGS) $(TEST_TARGET) 

# static pattern rule: multiple targets 

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< 

$(PROGS): %: %.o
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)


run_tests: $(PROGS)
	perl $(TESTS)

.PHONY: clean

clean:	
	rm -f $(PROGS) $(OBJS) test*.out 
