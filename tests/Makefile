#CC=clang
HASHDIR = ../src
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
        test82 test83 test84 test85 test86 test87 test88 test89 \
        test90 test91 test92 test93 test94 test95 test96
CFLAGS += -I$(HASHDIR)
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

all: $(PROGS) $(UTILS) $(PLAT_UTILS) keystat $(TEST_TARGET)

tests_only: $(PROGS) $(TEST_TARGET)

GITIGN = .gitignore
MKGITIGN = [ -f "$(GITIGN)" ] || echo "$(GITIGN)" > $(GITIGN); grep -q '^\$@$$' $(GITIGN) || echo "$@" >> $(GITIGN)

debug:
	$(MAKE) all HASH_DEBUG=1

pedantic:
	$(MAKE) all HASH_PEDANTIC=1

cplusplus:
	CC="$(CXX) -x c++" $(MAKE) all

thorough:
	$(MAKE) clean && $(MAKE) all EXTRA_CFLAGS='-pedantic'
	$(MAKE) clean && $(MAKE) all EXTRA_CFLAGS='-pedantic -DHASH_BLOOM=16'
	$(MAKE) clean && $(MAKE) tests_only EXTRA_CFLAGS='-pedantic -DHASH_BLOOM=16 -DHASH_DEBUG -DNO_DECLTYPE'
	$(MAKE) clean && CC="$(CXX) -x c++" $(MAKE) all EXTRA_CFLAGS='-pedantic'
	$(MAKE) clean && CC="$(CXX) -x c++" $(MAKE) all EXTRA_CFLAGS='-pedantic -DHASH_BLOOM=16'
	$(MAKE) clean && CC="$(CXX) -x c++" $(MAKE) tests_only EXTRA_CFLAGS='-pedantic -DHASH_BLOOM=16 -DHASH_DEBUG -DNO_DECLTYPE'

example: example.c $(HASHDIR)/uthash.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $(@).c

$(PROGS) $(UTILS) : $(HASHDIR)/uthash.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $(@).c
	@$(MKGITIGN)

hashscan : $(HASHDIR)/uthash.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $(@).c
	@$(MKGITIGN)

sleep_test : $(HASHDIR)/uthash.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_BLOOM=16 $(LDFLAGS) -o $@ $(@).c
	@$(MKGITIGN)

keystat : $(HASHDIR)/uthash.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_FUNCTION=HASH_BER $(LDFLAGS) -o keystat.BER keystat.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_FUNCTION=HASH_FNV $(LDFLAGS) -o keystat.FNV keystat.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_FUNCTION=HASH_JEN $(LDFLAGS) -o keystat.JEN keystat.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_FUNCTION=HASH_OAT $(LDFLAGS) -o keystat.OAT keystat.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_FUNCTION=HASH_SAX $(LDFLAGS) -o keystat.SAX keystat.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DHASH_FUNCTION=HASH_SFH $(LDFLAGS) -o keystat.SFH keystat.c

run_tests: $(PROGS)
	perl $(TESTS)

run_tests_mingw: $(PROGS)
	/bin/bash do_tests.mingw

astyle:
	astyle -n --style=kr --indent-switches --add-brackets *.c

.PHONY: clean astyle

clean:
	rm -f $(UTILS) $(PLAT_UTILS) $(PROGS) test*.out keystat.??? example hashscan sleep_test *.exe $(GITIGN)
	rm -rf *.dSYM
