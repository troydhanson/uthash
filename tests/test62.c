
#include <assert.h>
#include <stdlib.h>

#define HASH_FUNCTION(s, len, hashv) (hashv) = TrivialHash((const char *)s, len)
#include "uthash.h"

unsigned int TrivialHash(const char *s, size_t len)
{
    unsigned int h = 0;
    size_t i;
    for (i=0; i < len; ++i) {
        h += (unsigned char)s[i];
    }
    return h;
}

struct test_t {
    int a;
    int b;
    UT_hash_handle hh;
};

struct test_t *make_test(int value)
{
    struct test_t *test = (struct test_t *)malloc(sizeof *test);
    assert(test != NULL);
    test->a = value;
    return test;
}

int main()
{
    struct test_t *tests = NULL;
    struct test_t *test = NULL;
    int x;
    unsigned int h;

    x = 0x0042;
    HASH_VALUE(&x, sizeof x, h);
    assert(h == 0x42);

    x = 0x4002;
    HASH_VALUE(&x, sizeof x, h);
    assert(h == 0x42);

    test = make_test(0x0042);
    HASH_ADD_INT(tests, a, test);
    test = make_test(0x4002);
    HASH_ADD_INT(tests, a, test);

    x = 0x4002;
    test = NULL;
    HASH_FIND_BYHASHVALUE(hh, tests, &x, sizeof x, 0x42, test);
    assert(test != NULL);
    assert(test->a == 0x4002);

    x = 0x0042;
    test = NULL;
    HASH_FIND_BYHASHVALUE(hh, tests, &x, sizeof x, 0x42, test);
    assert(test != NULL);
    assert(test->a == 0x0042);

    x = 0x4002;
    test = NULL;
    HASH_FIND_BYHASHVALUE(hh, tests, &x, sizeof x, 0x43, test);
    assert(test == NULL);

    x = 0x0042;
    test = NULL;
    HASH_FIND_BYHASHVALUE(hh, tests, &x, sizeof x, 0x43, test);
    assert(test == NULL);

    x = 0x4003;
    test = NULL;
    HASH_FIND_BYHASHVALUE(hh, tests, &x, sizeof x, 0x42, test);
    assert(test == NULL);

    HASH_CLEAR(hh, tests);
}
