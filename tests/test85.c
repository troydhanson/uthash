#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

int main()
{
    int i;
    example_user_t *user, *users=NULL;

    /* create elements */
    for(i=0; i<10; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }

    size_t s = HASH_OVERHEAD(hh,users);
    printf("overhead %s\n", (s==0U)?"zero":"non-zero");
    HASH_CLEAR(hh,users);
    // should free those elements
    // but this test is not concerned with that
    s = HASH_OVERHEAD(hh,users);
    printf("overhead %s\n", (s==0U)?"zero":"non-zero");
    return 0;
}
