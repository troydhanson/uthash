#include <stdio.h>
#include <stdlib.h>

// Keep things all in one bucket, for simplicity.
#undef HASH_FUNCTION
#define HASH_FUNCTION(keyptr,keylen,hashv) (hashv) = 0

#include "uthash.h"

typedef struct {
    int id;
    UT_hash_handle srchh;
    UT_hash_handle dsthh;
} User;

#define ALWAYS_TRUE(x) 1

static User *new_user(int id)
{
    User *u = (User*)malloc(sizeof *u);
    u->id = id;
    return u;
}

int main(void)
{
    User *src = NULL;
    User *dst = NULL;
    User *u;
    User *tmp;
    int i;

    for (i = 0; i < 3; i++) {
        User *u = new_user(i);
        HASH_ADD(srchh, src, id, sizeof(int), u);
    }
    // "src" is [0,1,2] and "dst" is empty
    HASH_SELECT(dsthh, dst, srchh, src, ALWAYS_TRUE);
    printf("After HASH_SELECT:\n");
    HASH_ITER(srchh, src, u, tmp) {
        printf("src contains %d\n", u->id);
    }
    HASH_ITER(dsthh, dst, u, tmp) {
        printf("dst contains %d\n", u->id);
    }
    // Verify that dst->dsthh.tbl->tail has been updated.
    // Adding a new element to dst should put it after the last elt,
    // not after the first, and dst should end up containing [2,1,0,3].
    u = new_user(3);
    HASH_ADD(dsthh, dst, id, sizeof(int), u);
    printf("After HASH_ADD:\n");
    HASH_ITER(dsthh, dst, u, tmp) {
        printf("dst contains %d\n", u->id);
    }
    HASH_CLEAR(srchh, src);
    HASH_CLEAR(dsthh, dst);
    return 0;
}
