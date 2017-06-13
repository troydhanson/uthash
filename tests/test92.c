#include <stdio.h>

#define HASH_BLOOM 16
#define HASH_NOMEM_OK 1

#undef uthash_malloc
#undef uthash_free
#undef uthash_mem_failed
#define uthash_malloc(sz) alt_malloc(sz)
#define uthash_free(ptr,sz) alt_free(ptr)
#define uthash_mem_failed(e) do{((example_user_t*)e)->mem_failed=1;}while(0)
#define all_select(a) 1

#include "uthash.h"

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
    UT_hash_handle hh2;
    int mem_failed;
} example_user_t;

static int malloc_cnt = 0;
static int malloc_failed;
static int free_cnt = 0;

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

static void alt_free(void *ptr) {
    free_cnt++;
    free(ptr);
}

int main(int argc, char *argv[])
{

#define complain(a,c) do { \
    if (users) { \
        printf("%d: users hash must be empty\n", a); \
    } \
    if (user->hh.tbl) { \
        printf("%d hash table must be empty\n", a); \
    } \
    if (free_cnt != c) { \
        printf("%d Expected %d frees, only had %d\n", a, c, free_cnt); \
    } \
    if (user->mem_failed != 1) { \
        printf("%d Expected user->mem_failed(%d) to be 1\n", a, user->mem_failed); \
    } \
} while (0)

    example_user_t * users = 0, * user, * test, * users2 = 0;
    int id = 0, i, saved_cnt;

    malloc_cnt = 3; // bloom filter must fail
    user = (example_user_t*)malloc(sizeof(example_user_t));
    user->id = id;
    user->mem_failed = 0;
    user->hh.tbl = (UT_hash_table*)1;
    free_cnt = 0;
    HASH_ADD_INT(users, id, user);
    complain(1, 2);

    malloc_cnt = 2; // bucket creation must fail
    user->mem_failed = 0;
    free_cnt = 0;
    user->hh.tbl = (UT_hash_table*)1;
    HASH_ADD_INT(users, id, user);
    complain(2, 1);

    malloc_cnt = 1; // table creation must fail
    user->mem_failed = 0;
    free_cnt = 0;
    user->hh.tbl = (UT_hash_table*)1;
    HASH_ADD_INT(users, id, user);
    complain(3, 0);

    malloc_cnt = 4; // hash must create OK
    user->mem_failed = 0;
    HASH_ADD_INT(users, id, user);
    if (user->mem_failed) {
        printf("mem failed flags must be 0\n");
    }
    HASH_FIND_INT(users,&id,test);
    if (!test) {
        printf("test user ID %d not found\n", id);
    }

    // let's add users until expansion fails.
    free_cnt = 0;
    while (1) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if ((user->id = ++id) == 1000) {
            // prevent infinite, or too long of a loop here
            printf("too many allocs before memory request");
            break;
        }
        user->hh.tbl = (UT_hash_table*)1;
        HASH_ADD_INT(users, id, user);
        if (malloc_failed) {

            if (free_cnt != 0) {
                printf("something unexpected was freed\n");
            }

            if (id < 10) {
                printf("there is no way your bucket size is <= 10\n");
            }

            if (user->hh.tbl || !user->mem_failed) {
                printf("mem_failed must be 1 after extension failed\n");
            }

            // let's make sure all previous IDs are there.
            for (int i=0; i<id; i++) {
                HASH_FIND_INT(users,&i,test);
                if (!test) {
                    printf("test user ID %d not found\n", i);
                }
            }

            // let's try to add again, but with mem_failed set to 0
            user->hh.tbl = 0;
            user->mem_failed = 0;
            malloc_failed = 0;
            HASH_ADD_INT(users, id, user);
            if (!malloc_failed) {
                printf("malloc not attempted\n");
            }
            if (!user->mem_failed || user->hh.tbl) {
                printf("mem_failed must be 1 after extension failed second time\n");
            }

            break;
        }
    }

    // let's test HASH_SELECT.
    // let's double the size of the table we've already built.
    saved_cnt = id;

    for (i=0; i<saved_cnt; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        user->id = ++id;
        malloc_cnt = 4;
        HASH_ADD_INT(users, id, user);
    }

    malloc_cnt = 0;
    free_cnt = 0;
    HASH_SELECT(hh2, users2, hh, users, all_select);
    if (users2) {
        printf("Nothing could have been copied into users2\n");
    }
    HASH_ITER(hh, users, user, test) {
        if (user->hh2.tbl) {
            printf("User ID %d has tbl at %p\n", user->id, (void*)user->hh2.tbl);
        }
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

    // printf("users2 size is %d, users is %d\n", HASH_CNT(hh2, users2), HASH_COUNT(users));
    saved_cnt = 0;
    HASH_ITER(hh, users, user, test) {
        example_user_t * user2;
        HASH_FIND(hh2, users2, &user->id, sizeof(int), user2);
        if (user2) {
            if (!user->hh2.tbl) {
                printf("User ID %d has tbl at %p, expected !0\n", user->id, (void*)user->hh2.tbl);
            }
        } else {
            saved_cnt++;
            if (user->hh2.tbl) {
                printf("User ID %d has tbl at %p, expected 0\n", user->id, (void*)user->hh2.tbl);
            }
        }
    }

    if (saved_cnt + HASH_CNT(hh2, users2) != HASH_COUNT(users)) {
        printf("Selected elements : %d + %d != %d\n",
                saved_cnt, HASH_CNT(hh2, users2), HASH_COUNT(users));
    }

    printf("END\n");

    return 0;

}

