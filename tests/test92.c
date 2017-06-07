
#define HASH_BLOOM 16

#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

#define select_all(a) 1

typedef struct {
    int id;
    UT_hash_handle hh;
    UT_hash_handle ah;
} example_user_t;

int main(int argc,char *argv[])
{
    int i;
    example_user_t *user, *users=NULL, *ausers=NULL;

    user = (example_user_t*)malloc(sizeof(example_user_t));
    if (!user) {
        return 1;
    }

    user->id = i = 1;
    HASH_ADD_INT(users, id, user);
    HASH_SELECT(ah, ausers, hh, users, select_all);
    HASH_FIND(ah, ausers, &i, sizeof(i), user);
    if (!user) {
        printf("selected user not found in destination table, bloom failed?\n");
    }

    printf("END\n");

    return 0;
}
