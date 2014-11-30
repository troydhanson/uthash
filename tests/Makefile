#CC=clang
HASHDIR = ../src
FUNCS = BER SAX FNV OAT JEN SFH 
SPECIAL_FUNCS = MUR
UTILS = emit_keys
PROGS = test1 test2 test3 test4 test5 test6 test7 test8 test9   \
		    test10 test11 test12 test13 test14 test15 test16 test17 \
        test18 test19 test20 test21 test22 test23 test24 test25 \
        test26 test27 test28 test29 test30 test31 test32 test33 \
        test34 test35 test36 test37 test38 test39 test40 test41 \
        test42 test43 test44 test45 test46 test47 test48 test49 \
				test50 test51 test52 test53 test54 test55 test56 test57 \
        test58 test59 test60 test61 test62 test63 test64 test65 \
        test66 test67 test68 test69 test70 test71 test72 test73 \
        test74 test75 test76 test77 test78 test79 test80 test81 \
        test82 test83 test84 test85
CFLAGS = -I$(HASHDIR) 
#CFLAGS += -DHASH_BLOOM=16
#CFLAGS += -O2
CFLAGS += -g
#CFLAGS += -Wstrict-aliasing=2
CFLAGS += -Wall 
#CFLAGS += -Wextra 
#CFLAGS += -std=c89
CFLAGS += ${EXTRA_CFLAGS}

ifeq ($(HASH_DEBUG),1)
CFLAGS += -DHASH_DEBUG=1
endif

ifeq ($(HASH_PEDANTIC),1)
CFLAGS += -pedantic 
endif

TEST_TARGET=run_tests
TESTS=./do_tests

MUR_CFLAGS = -DHASH_USING_NO_STRICT_ALIASING -fno-strict-aliasing
# On GNU we use -fno-strict-aliasing when using the Murmurhash
ifneq ($(strip $(shell $(CC) -v 2>&1 |egrep "gcc")),)
ifeq ($(HASH_FUNCTION),"HASH_MUR")
  CFLAGS += $(MUR_CFLAGS)
endif
endif

# detect Cygwin
ifneq ($(strip $(shell $(CC) -v 2>&1 |grep "cygwin")),)
  TESTS=./do_tests.cygwin
endif

# detect MinGW
ifneq ($(strip $(shell $(CC) -v 2>&1 |grep "mingw")),)
  TEST_TARGET=run_tests_mingw
  TESTS=./do_tests.mingw
endif

#detect Linux (platform specific utilities)
ifneq ($(strip $(shell $(CC) -v 2>&1 |grep "linux")),)
  PLAT_UTILS = hashscan sleep_test
endif

#detect FreeBSD (platform specific utilities)
ifeq ($(strip $(shell uname -s)), FreeBSD)
  ifeq ($(shell if [ `sysctl -n kern.osreldate` -ge 0801000 ]; then echo "ok"; fi), ok)
    PLAT_UTILS = hashscan sleep_test
  endif
endif


all: $(PROGS) $(UTILS) $(PLAT_UTILS) $(FUNCS) $(SPECIAL_FUNCS) $(TEST_TARGET) 

tests_only: $(PROGS) $(TEST_TARGET)

debug:
	$(MAKE) all HASH_DEBUG=1

pedantic:
	$(MAKE) all HASH_PEDANTIC=1

cplusplus:
	CC=$(CXX) $(MAKE) all 

example: example.c $(HASHDIR)/uthash.h
	$(CC) $(CFLAGS) -o $@ $(@).c 

$(PROGS) $(UTILS) : $(HASHDIR)/uthash.h
	$(CC) $(CFLAGS) -o $@ $(@).c 

hashscan : $(HASHDIR)/uthash.h
	$(CC) $(CFLAGS) $(MUR_CFLAGS) -o $@ $(@).c 

sleep_test : $(HASHDIR)/uthash.h 
	$(CC) $(CFLAGS) -DHASH_BLOOM=16 -o $@ $(@).c 

$(FUNCS) : $(HASHDIR)/uthash.h
	$(CC) $(CFLAGS) -DHASH_FUNCTION=HASH_$@ -o keystat.$@ keystat.c 

$(SPECIAL_FUNCS) : $(HASHDIR)/uthash.h
	$(CC) $(CFLAGS) $(MUR_CFLAGS) -DHASH_FUNCTION=HASH_$@ -o keystat.$@ keystat.c 

run_tests: $(PROGS)
	perl $(TESTS)

run_tests_mingw: $(PROGS)
	/bin/sh do_tests.mingw

.PHONY: clean

clean:	
	rm -f $(UTILS) $(PLAT_UTILS) $(PROGS) test*.out keystat.??? example *.exe
	rm -rf *.dSYM
