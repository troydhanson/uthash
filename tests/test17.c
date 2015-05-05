#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

static int rev(void *_a, void *_b)
{
    example_user_t *a = (example_user_t*)_a;
    example_user_t *b = (example_user_t*)_b;
    printf("called for a:%d, b:%d\n",a->id, b->id);
    return (a->id - b->id);
}

int main(int argc,char *argv[])
{
    int i;
    example_user_t *user, *users=NULL;

    /* create elements */
    for(i=9; i>=0; i--) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }

    for(user=users; user != NULL; user=(example_user_t*)user->hh.next) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }
    printf("sorting\n");
    HASH_SORT(users,rev);
    for(user=users; user != NULL; user=(example_user_t*)user->hh.next) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }

    printf("adding 10-20\n");
    for(i=20; i>=10; i--) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }
    for(user=users; user != NULL; user=(example_user_t*)user->hh.next) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }
    printf("sorting\n");
    HASH_SORT(users,rev);
    for(user=users; user != NULL; user=(example_user_t*)user->hh.next) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }
    return 0;
}
