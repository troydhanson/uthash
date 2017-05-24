#include <stdio.h>
#include <setjmp.h>

#define HASH_BLOOM 16

#include "uthash.h"

#undef uthash_malloc
#undef uthash_fatal
#define uthash_malloc(sz) alt_malloc(sz)
#define uthash_fatal(s) alt_fatal(s)

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

static int malloc_cnt = 0;
static int malloc_failed;
static int is_fatal;
static jmp_buf j_buf;

static void *alt_malloc(size_t sz)
{
    if (--malloc_cnt <= 0) {
        // printf("fail malloc req for %ld\n", sz);
        malloc_failed = 1;
        return 0;
    }
    // printf("OK malloc req for %ld\n", sz);
    malloc_failed = 0;
    return malloc(sz);
}

static void alt_fatal(char * s) {
    // printf("alt_fatal: %s\n", s);
    is_fatal = 1;
    longjmp(j_buf, 1);
}

int main(int argc, char *argv[])
{

#define init(a) do { \
    users = 0; \
    user = malloc(sizeof(example_user_t)); \
    user->id = id; \
    is_fatal = 0; \
    malloc_cnt = a; \
    /* printf("adding to hash...\n"); */ \
    if (!setjmp(j_buf)) { \
        HASH_ADD_INT(users, id, user); \
    } \
} while(0)

    // $TODO: this test doesn't work because
    // uthash_fatal can't successfully return - it
    // makes uthash write into *0. This either needs to
    // be broken up into small test files, or use threads,
    // a thread can hard exit. But there are no threads 
    // in other tests, which means compilation changes, etc.

    example_user_t * users, * user;
    int id = 0;

    init(3); // bloom filter must fail
    if (!is_fatal) {
        printf("fatal not called after bloom failure\n");
    }

    init(2); // bucket creation must fail
    if (!is_fatal) {
        printf("fatal not called after bucket creation failure\n");
    }

    init(1); // table creation must fail
    if (!is_fatal) {
        printf("fatal not called after table creation failure\n");
    }

    init(4); // hash must create OK
    if (is_fatal) {
        printf("fatal error when creating hash normally\n");
        // bad idea to continue running
        return 1;
    }

    // let's add users until expansion fails.
    users = 0;
    malloc_cnt = 4;
    while (1) {
        user = malloc(sizeof(example_user_t));
        user->id = id;
        if (id++ == 1000) {
            printf("there is no way 1000 iterations didn't require realloc\n");
            break;
        }
        if (!setjmp(j_buf)) {
            HASH_ADD_INT(users, id, user);
        }
        malloc_cnt = 0;
        if (malloc_failed) {

            if (!is_fatal) {
                printf("fatal not called after bucket not extended\n");
            }
            if (id < 10) {
                printf("there is no way your bucket size is 10\n");
            }

            // we can't really do anything, the hash is not in consistent
            // state, so assume this is a success.
            break;

        }
    }

    printf("End\n");

    return 0;

}
