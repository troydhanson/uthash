#include <assert.h>
#include <stdio.h>
#include "utstring.h"

int findl(const UT_string *haystack, int pos, char needle) {
  return utstring_find(haystack, pos, &needle, 1);
}
int findr(const UT_string *haystack, int pos, char needle) {
  return utstring_findR(haystack, pos, &needle, 1);
}

int main()
{
    UT_string *s;

    utstring_new(s);
    utstring_printf(s, "%s %s!", "hello", "world");
    assert(utstring_len(s) == 12);

    printf("F:");
    for (int i = -12; i <= 12; ++i) {
      printf(" %d", findl(s, i, 'l'));
    }
    printf("\n");

    printf("R:");
    for (int i = -12; i <= 12; ++i) {
      printf(" %d", findr(s, i, 'l'));
    }
    printf("\n");

    utstring_free(s);
    return 0;
}
