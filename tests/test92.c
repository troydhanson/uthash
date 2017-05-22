#include <stdio.h>

#define HASH_BLOOM 16
#define HASH_NOMEM_OK

#undef uthash_malloc
#undef uthash_free
#undef uthash_mem_failed
#define uthash_malloc(sz) alt_malloc(sz)
#define uthash_free(ptr,sz) alt_free(ptr)
#define uthash_mem_failed(e) do{e->mem_failed=1;}while(0)

#include "uthash.h"

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
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
    if (!user->hh.mem_failed) { \
        printf("%d mem_failed must be 1\n", a); \
    } \
    if (free_cnt != c) { \
        printf("%d Expected %d frees, only had %d\n", a, c, free_cnt); \
    } \
    if (user->mem_failed != 1) { \
        printf("%d Expected user->mem_failed(%d) to be 1\n", a, user->mem_failed); \
    } \
} while (0)

    example_user_t * users = 0, * user, * test;
    int id = 0;

    malloc_cnt = 3; // bloom filter must fail
    user = malloc(sizeof(example_user_t));
    user->id = id;
    user->mem_failed = user->hh.mem_failed = 0;
    free_cnt = 0;
    HASH_ADD_INT(users, id, user);
    complain(1, 2);

    malloc_cnt = 2; // bucket creation must fail
    user->mem_failed = user->hh.mem_failed = 0;
    free_cnt = 0;
    HASH_ADD_INT(users, id, user);
    complain(2, 1);

    malloc_cnt = 1; // table creation must fail
    user->mem_failed = user->hh.mem_failed = 0;
    free_cnt = 0;
    HASH_ADD_INT(users, id, user);
    complain(3, 0);

    malloc_cnt = 4; // hash must create OK
    user->mem_failed = 0;
    HASH_ADD_INT(users, id, user);
    if (user->mem_failed || user->hh.mem_failed) {
        printf("mem failed flags must be 0\n");
    }
    HASH_FIND_INT(users,&id,test);
    if (!test) {
        printf("test user ID %d not found\n", id);
    }

    // let's add users until expansion fails.
    free_cnt = 0;
    while (1) {
        user = malloc(sizeof(example_user_t));
        user->id = ++id;
        user->hh.mem_failed = 1; // to make sure it's reset to 0
        HASH_ADD_INT(users, id, user);
        if (malloc_failed) {

            if (free_cnt != 0) {
                printf("something unexpected was freed\n");
            }

            if (id < 10) {
                printf("there is no way your bucket size is <= 10\n");
            }

            if (!user->hh.mem_failed || !user->mem_failed) {
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
            user->hh.mem_failed = 0;
            user->mem_failed = 0;
            malloc_failed = 0;
            HASH_ADD_INT(users, id, user);
            if (!malloc_failed) {
                printf("malloc not attempted\n");
            }
            if (!user->mem_failed || !user->hh.mem_failed) {
                printf("mem_failed must be 1 after extension failed second time\n");
            }

            break;
        }
    }

    printf("OK\n");

    return 0;

}

