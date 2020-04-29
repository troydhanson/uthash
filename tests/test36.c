#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct {
    int id;
    UT_hash_handle hh;
    UT_hash_handle ah;
} example_user_t;

#define EVENS(x) (((x)->id % 2) == 0)
static int evens(void *userv)
{
    example_user_t *user = (example_user_t*)userv;
    return ((user->id % 2) ? 0 : 1);
}

static int idcmp(void *_a, void *_b)
{
    example_user_t *a = (example_user_t*)_a;
    example_user_t *b = (example_user_t*)_b;
    return (a->id - b->id);
}

int main()
{
    int i;
    example_user_t *user, *users=NULL, *ausers=NULL;

    /* create elements */
    for(i=0; i<10; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        user->id = i;
        HASH_ADD_INT(users,id,user);
    }

    for(user=users; user!=NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d\n", user->id);
    }

    /* now select some users into ausers */
    HASH_SELECT(ah,ausers,hh,users,evens);
    HASH_SRT(ah,ausers,idcmp);

    for(user=users; user!=NULL; user=(example_user_t*)(user->hh.next)) {
        example_user_t *found = NULL;
        int should_find = !!evens(user);
        HASH_FIND(ah, ausers, &user->id, sizeof(user->id), found);
        printf("user %d, should_find=%d, found=%d\n", user->id, should_find, (int)(!!found));
    }

    for(user=ausers; user!=NULL; user=(example_user_t*)(user->ah.next)) {
        printf("auser %d\n", user->id);
    }

    return 0;
}
