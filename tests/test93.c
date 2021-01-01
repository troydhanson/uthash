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
static example_user_t * users;
static int user_id = 0;

static void *alt_malloc(size_t sz)
{
    if (--malloc_cnt <= 0) {
        malloc_failed = 1;
        return 0;
    }
    malloc_failed = 0;
    return malloc(sz);
}

static void alt_fatal(char const * s) {
    (void)s;
    is_fatal = 1;
    longjmp(j_buf, 1);
}

static void init_users(int need_malloc_cnt) {
    users = NULL;
    example_user_t * user = (example_user_t*)malloc(sizeof(example_user_t));
    user->id = user_id;
    is_fatal = 0;
    malloc_cnt = need_malloc_cnt;
    if (!setjmp(j_buf)) {
        HASH_ADD_INT(users, id, user);
    } else {
        free(user);
    }
}

int main()
{
    example_user_t *user;

    init_users(3); /* bloom filter must fail */
    if (!is_fatal) {
        printf("fatal not called after bloom failure\n");
    }

    init_users(2); /* bucket creation must fail */
    if (!is_fatal) {
        printf("fatal not called after bucket creation failure\n");
    }

    init_users(1); /* table creation must fail */
    if (!is_fatal) {
        printf("fatal not called after table creation failure\n");
    }

    init_users(4); /* hash must create OK */
    if (is_fatal) {
        printf("fatal error when creating hash normally\n");
        /* bad idea to continue running */
        return 1;
    }

    /* let's add users until expansion fails */
    users = NULL;
    malloc_cnt = 4;
    while (1) {
        if (user_id++ == 1000) {
            printf("there is no way 1000 iterations didn't require realloc\n");
            break;
        }
        user = (example_user_t*)malloc(sizeof(example_user_t));
        user->id = user_id;
        if (!setjmp(j_buf)) {
            HASH_ADD_INT(users, id, user);
        } else {
            free(user);
        }
        if (malloc_failed) {

            if (!is_fatal) {
                printf("fatal not called after bucket not extended\n");
            }
            if (user_id < 10) {
                printf("there is no way your bucket size is 10\n");
            }

            /* we can't really do anything, the hash is not in consistent
             * state, so assume this is a success. */
            break;
        }
        malloc_cnt = 0;
    }

    HASH_CLEAR(hh, users);

    printf("End\n");
    return 0;
}
