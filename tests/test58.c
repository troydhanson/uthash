#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

int main(int argc,char *argv[])
{
    int i;
    unsigned c;
    example_user_t *user, *tmp, *users=NULL;

    /* create elements */
    for(i=0; i<10; i++) {
        if ( (user = (example_user_t*)malloc(sizeof(example_user_t))) == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }
    /* show the hash */
    for(user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }


    c = HASH_COUNT(users);
    printf("%u users. Deleting odd id's...\n", c);
    /* delete the odd id's */
    HASH_ITER(hh, users, user, tmp) {
        if ((user->id & 1) != 0) {
            HASH_DEL(users,user);
        }
    }

    /* show the hash */
    for(user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }

    c = HASH_COUNT(users);
    printf("%u users. Deleting remaining id's...\n", c);
    /* delete all that are left */
    HASH_ITER(hh, users, user, tmp) {
        HASH_DEL(users,user);
    }

    c = HASH_COUNT(users);
    printf("%u users.\n", c);
    /* show the hash */
    for(user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }
    return 0;
}
