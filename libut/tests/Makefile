PROGS=test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 \
      test13 test14 test15 test16 test17 test18 test19
OBJS=$(patsubst %,%.o,$(PROGS))

CFLAGS += -I../include
CFLAGS += -g
CFLAGS += -Wall -Wextra
LDFLAGS += -L.. -lut

TEST_TARGET=run_tests
TESTS=./do_tests

all: $(OBJS) $(PROGS) $(TEST_TARGET) 

# static pattern rule: multiple targets 

$(OBJS): %.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

$(PROGS): %: %.o
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS)


run_tests: $(PROGS)
	perl $(TESTS)

.PHONY: clean

clean:	
	rm -f $(PROGS) $(OBJS) test*.out 
