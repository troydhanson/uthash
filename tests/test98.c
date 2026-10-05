#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "utstring.h"

int findl(const UT_string *haystack, int pos, char needle) {
  return utstring_find(haystack, pos, &needle, 1);
}
int findr(const UT_string *haystack, int pos, char needle) {
  return utstring_findR(haystack, pos, &needle, 1);
}

/* Compare every candidate byte range, independently of the KMP implementation. */
static long reference_find(const char *haystack, long len, long pos,
                           const char *needle, size_t needle_len, int reverse) {
  long i;
  if (pos < 0) {
    pos += len;
  }
  if (reverse) {
    for (i = pos + 1 - (long)needle_len; i >= 0; --i) {
      if (memcmp(haystack + i, needle, needle_len) == 0) {
        return i;
      }
    }
  } else {
    for (i = pos; i + (long)needle_len <= len; ++i) {
      if (memcmp(haystack + i, needle, needle_len) == 0) {
        return i;
      }
    }
  }
  return -1;
}

static int check_search(const UT_string *s, long pos, const char *needle,
                        size_t needle_len, long expected_f, long expected_r) {
  long actual_f = utstring_find(s, pos, needle, needle_len);
  long actual_r = utstring_findR(s, pos, needle, needle_len);
  size_t i;
  if (actual_f == expected_f && actual_r == expected_r) {
    return 0;
  }
  fprintf(stderr, "pos=%ld F: expected %ld got %ld; R: expected %ld got %ld\n",
          pos, expected_f, actual_f, expected_r, actual_r);
  fprintf(stderr, "haystack:");
  for (i = 0; i < utstring_len(s); ++i) {
    fprintf(stderr, " %02x", (unsigned char)utstring_body(s)[i]);
  }
  fprintf(stderr, "\nneedle:");
  for (i = 0; i < needle_len; ++i) {
    fprintf(stderr, " %02x", (unsigned char)needle[i]);
  }
  fprintf(stderr, "\n");
  return 1;
}

static int test_cases(UT_string *s) {
  static const struct {
    const char *haystack;
    size_t len;
    long pos;
    const char *needle;
    size_t needle_len;
    long forward;
    long reverse;
  } cases[] = {
    {"abc", 3, 0, "abc", 3, 0, -1},
    {"abc", 3, 2, "abc", 3, -1, 0},
    {"abc", 3, -3, "abc", 3, 0, -1},
    {"abc", 3, -1, "abc", 3, -1, 0},
    {"abc", 3, 2, "c", 1, 2, 2},
    {"abc", 3, -1, "c", 1, 2, 2},
    {"a\0b", 3, 1, "\0", 1, 1, 1},
    {"a\0b", 3, -2, "\0", 1, 1, 1},
    {"a\0b", 3, 2, "\0b", 2, -1, 1},
    {"a\0b", 3, -3, "a\0", 2, 0, -1},
    {"aaaa", 4, 3, "aa", 2, -1, 2},
    {"aaaa", 4, 2, "aa", 2, 2, 1},
    {"aaaa", 4, 1, "aa", 2, 1, 0},
    {"aaaa", 4, 0, "aa", 2, 0, -1},
    {"ababa", 5, 4, "aba", 3, -1, 2},
    {"ababa", 5, 3, "aba", 3, -1, 0},
    {"ababa", 5, 2, "aba", 3, 2, 0},
    {"ababa", 5, 0, "aba", 3, 0, -1},
    {"abc", 3, 1, "abc", 3, -1, -1},
    {"abc", 3, 2, "abcd", 4, -1, -1},
    {"\0\0", 2, 1, "\0\0", 2, -1, 0},
    {"a\0a\0a", 5, 4, "a\0a", 3, -1, 2}
  };
  size_t i;
  for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    utstring_clear(s);
    utstring_bincpy(s, cases[i].haystack, cases[i].len);
    if (check_search(s, cases[i].pos, cases[i].needle, cases[i].needle_len,
                     cases[i].forward, cases[i].reverse)) {
      return 1;
    }
  }
  printf("Cases: %u\n", (unsigned)(sizeof(cases) / sizeof(cases[0])));
  return 0;
}

static unsigned word_count(size_t len) {
  unsigned count = 1;
  size_t i;
  for (i = 0; i < len; ++i) {
    count *= 3;
  }
  return count;
}

static void make_word(char *word, size_t len, unsigned number) {
  static const char alphabet[] = {'\0', 'a', 'b'};
  size_t i;
  for (i = 0; i < len; ++i) {
    word[i] = alphabet[number % 3];
    number /= 3;
  }
}

static int test_exhaustive(UT_string *s) {
  char haystack[5], needle[5];
  size_t len, needle_len;
  unsigned h, n;
  long pos;
  unsigned long checks = 0;

  /* Nonempty byte strings and only the documented positions [-len, len). */
  for (len = 1; len <= sizeof(haystack); ++len) {
    for (h = 0; h < word_count(len); ++h) {
      make_word(haystack, len, h);
      utstring_clear(s);
      utstring_bincpy(s, haystack, len);
      for (needle_len = 1; needle_len <= sizeof(needle); ++needle_len) {
        for (n = 0; n < word_count(needle_len); ++n) {
          make_word(needle, needle_len, n);
          for (pos = -(long)len; pos < (long)len; ++pos) {
            long expected_f = reference_find(haystack, (long)len, pos,
                                             needle, needle_len, 0);
            long expected_r = reference_find(haystack, (long)len, pos,
                                             needle, needle_len, 1);
            if (check_search(s, pos, needle, needle_len, expected_f, expected_r)) {
              return 1;
            }
            checks += 2;
          }
        }
      }
    }
  }
  printf("Exhaustive: %lu searches\n", checks);
  return 0;
}

int main()
{
    UT_string *s;

    utstring_new(s);
    utstring_printf(s, "%s %s!", "hello", "world");
    assert(utstring_len(s) == 12);

    printf("F:");
    for (int i = -12; i < 12; ++i) {
      printf(" %d", findl(s, i, 'l'));
    }
    printf("\n");

    printf("R:");
    for (int i = -12; i < 12; ++i) {
      printf(" %d", findr(s, i, 'l'));
    }
    printf("\n");

    if (test_cases(s) || test_exhaustive(s)) {
      utstring_free(s);
      return 1;
    }

    utstring_free(s);
    return 0;
}
