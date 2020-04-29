#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct {
    int id;
    UT_hash_handle hh;
    UT_hash_handle ah;
} example_user_t;

#define EVENS(x) ((((example_user_t*)(x))->id % 2) == 0)

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
    printf("users count: %u\n", HASH_CNT(hh,users));

    /* now select some users into ausers */
    HASH_SELECT(ah,ausers,hh,users,EVENS);
    HASH_SRT(ah,ausers,idcmp);

    for(user=ausers; user!=NULL; user=(example_user_t*)(user->ah.next)) {
        printf("auser %d\n", user->id);
    }
    printf("ausers count: %u\n", HASH_CNT(ah,ausers));
    HASH_CLEAR(ah,ausers);
    printf("cleared ausers.\n");
    printf("ausers count: %u\n", HASH_CNT(ah,ausers));
    for(user=ausers; user!=NULL; user=(example_user_t*)(user->ah.next)) {
        printf("auser %d\n", user->id);
    }
    printf("users count: %u\n", HASH_CNT(hh,users));
    return 0;
}
