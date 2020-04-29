#include <stdio.h>

#define HASH_NONFATAL_OOM 1

#include "uthash.h"

#undef uthash_malloc
#undef uthash_free
#undef uthash_nonfatal_oom
#define uthash_malloc(sz) alt_malloc(sz)
#define uthash_free(ptr,sz) alt_free(ptr)
#define uthash_nonfatal_oom(e) do{(e)->mem_failed=1;}while(0)
#define all_select(a) 1

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
    UT_hash_handle hh2;
    int mem_failed;
} example_user_t;

static int malloc_cnt = 0;
static int malloc_failed = 0;
static int free_cnt = 0;

static void *alt_malloc(size_t sz)
{
    if (--malloc_cnt <= 0) {
        malloc_failed = 1;
        return 0;
    }
    malloc_failed = 0;
    return malloc(sz);
}

static void alt_free(void *ptr) {
    free_cnt++;
    free(ptr);
}

static void complain(int index, example_user_t *users, example_user_t *user)
{
    int expected_frees = (3 - index);
    if (users) {
        printf("%d: users hash must be empty\n", index);
    }
    if (user->hh.tbl) {
        printf("%d hash table must be empty\n", index);
    }
    if (free_cnt != expected_frees) {
        printf("%d Expected %d frees, only had %d\n", index, expected_frees, free_cnt);
    }
    if (user->mem_failed != 1) {
        printf("%d Expected user->mem_failed(%d) to be 1\n", index, user->mem_failed);
    }
}

int main()
{
    example_user_t *users = NULL;
    example_user_t *user = (example_user_t*)malloc(sizeof(example_user_t));
    example_user_t *test;
    example_user_t *users2 = NULL;
    int id = 0;
    int i;
    int saved_cnt;

    user->id = id;

#ifdef HASH_BLOOM
    malloc_cnt = 3; // bloom filter must fail
    user->mem_failed = 0;
    user->hh.tbl = (UT_hash_table*)1;
    free_cnt = 0;
    HASH_ADD_INT(users, id, user);
    complain(1, users, user);
#endif /* HASH_BLOOM */

    malloc_cnt = 2; // bucket creation must fail
    user->mem_failed = 0;
    free_cnt = 0;
    user->hh.tbl = (UT_hash_table*)1;
    HASH_ADD_INT(users, id, user);
    complain(2, users, user);

    malloc_cnt = 1; // table creation must fail
    user->mem_failed = 0;
    free_cnt = 0;
    user->hh.tbl = (UT_hash_table*)1;
    HASH_ADD_INT(users, id, user);
    complain(3, users, user);

    malloc_cnt = 4; // hash must create OK
    user->mem_failed = 0;
    HASH_ADD_INT(users, id, user);
    if (user->mem_failed) {
        printf("mem_failed must be 0, not %d\n", user->mem_failed);
    }
    HASH_FIND_INT(users,&id,test);
    if (!test) {
        printf("test user ID %d not found\n", id);
    }

    if (HASH_COUNT(users) != 1) {
        printf("Got HASH_COUNT(users)=%d, should be 1\n", HASH_COUNT(users));
    }

    // let's add users until expansion fails.
    malloc_failed = 0;
    free_cnt = 0;
    malloc_cnt = 1;
    for (id = 1; 1; ++id) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        user->id = id;
        if (id >= 1000) {
            // prevent infinite, or too long of a loop here
            puts("too many allocs before memory request");
            break;
        }
        user->hh.tbl = (UT_hash_table*)1;
        HASH_ADD_INT(users, id, user);
        if (malloc_failed) {
            if (id < 10) {
                puts("there is no way your bucket size is <= 10");
            }

            if (user->hh.tbl) {
                puts("user->hh.tbl should be NULL after failure");
            } else if (user->mem_failed != 1) {
                printf("mem_failed should be 1 after failure, not %d\n", user->mem_failed);
            }

            if (free_cnt != 0) {
                printf("Expected 0 frees, had %d\n", free_cnt);
            }

            // let's make sure all previous IDs are there.
            for (i=0; i<id; ++i) {
                HASH_FIND_INT(users,&i,test);
                if (test == NULL) {
                    printf("test user ID %d not found\n", i);
                }
            }

            // let's try to add again, but with mem_failed set to 0
            user->hh.tbl = NULL;
            user->mem_failed = 0;
            malloc_failed = 0;
            HASH_ADD_INT(users, id, user);
            if (!malloc_failed) {
                puts("malloc should have been attempted");
            }
            if (user->hh.tbl) {
                puts("user->hh.tbl should be NULL after second failure");
            } else if (user->mem_failed != 1) {
                printf("mem_failed should be 1 after second failure, not %d\n", user->mem_failed);
            }

            break;
        }
    }

    // let's test HASH_SELECT.
    // let's double the size of the table we've already built.
    saved_cnt = id;

    if (HASH_COUNT(users) != (unsigned)saved_cnt) {
        printf("Got HASH_COUNT(users)=%d, should be %d\n", HASH_COUNT(users), saved_cnt);
    }

    for (i=0; i < saved_cnt; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        user->id = ++id;
        malloc_cnt = 20;  // don't fail
        HASH_ADD_INT(users, id, user);
    }

    HASH_ITER(hh, users, user, test) {
        user->mem_failed = 0;
    }

// HASH_SELECT calls uthash_nonfatal_oom() with an argument of type (void*).
#undef uthash_nonfatal_oom
#define uthash_nonfatal_oom(e) do{((example_user_t*)e)->mem_failed=1;}while(0)

    malloc_cnt = 0;
    free_cnt = 0;
    HASH_SELECT(hh2, users2, hh, users, all_select);
    if (users2) {
        puts("Nothing should have been copied into users2");
    }
    HASH_ITER(hh, users, user, test) {
        if (user->hh2.tbl) {
            printf("User ID %d has tbl at %p\n", user->id, (void*)user->hh2.tbl);
        }
        if (user->mem_failed != 1) {
            printf("User ID %d has mem_failed(%d), should be 1\n", user->id, user->mem_failed);
        }
        user->mem_failed = 0;
    }

    malloc_cnt = 4;
    HASH_SELECT(hh2, users2, hh, users, all_select);

    // note about the above.
    // we tried to stick up to 1,000 entries into users,
    // and the malloc failed after saved_cnt. The bucket threshold must have
    // been triggered. We then doubled the amount of entries in user,
    // and just ran HASH_SELECT, trying to copy them into users2.
    // because the order is different, and because we continue after
    // failures, the bucket threshold may get triggered on arbitrary
    // elements, depending on the hash function.

    saved_cnt = 0;
    HASH_ITER(hh, users, user, test) {
        example_user_t * user2;
        HASH_FIND(hh2, users2, &user->id, sizeof(int), user2);
        if (user2) {
            if (!user->hh2.tbl) {
                printf("User ID %d has tbl==NULL\n", user->id);
            }
            if (user->mem_failed != 0) {
                printf("User ID %d has mem_failed(%d), expected 0\n", user->id, user->mem_failed);
            }
        } else {
            saved_cnt++;
            if (user->hh2.tbl) {
                printf("User ID %d has tbl at %p, expected 0\n", user->id, (void*)user->hh2.tbl);
            }
            if (user->mem_failed != 1) {
                printf("User ID %d has mem_failed(%d), expected is 1\n", user->id, user->mem_failed);
            }
        }
    }

    if (saved_cnt + HASH_CNT(hh2, users2) != HASH_COUNT(users)) {
        printf("Selected elements : %d + %d != %d\n",
                saved_cnt, HASH_CNT(hh2, users2), HASH_COUNT(users));
    }

    puts("End");

    return 0;
}
