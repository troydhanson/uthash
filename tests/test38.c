#include "uthash.h"
#include <stdlib.h>
#include <stdio.h>

struct test_t {
  int a;
  UT_hash_handle hh;
};

struct test_t *tests=NULL, *test;

int main(void) {
  int a, b;
  for (b=0; b < 3; b++) {
    for (a=0; a < 10; a++) {
      test = NULL;
      HASH_FIND(hh, tests, &a, sizeof(a), test);
      if (! test) {
        test = (struct test_t*)malloc(sizeof(struct test_t));
        memset(test, 0, sizeof(struct test_t));
        test->a = a;
        HASH_ADD(hh, tests, a, sizeof(a), test);
      }
    }
  }
  printf("hash count %u\n", HASH_COUNT(tests));
  return 0;
}
