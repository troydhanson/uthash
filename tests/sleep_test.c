#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */
#include <unistd.h>   /* getpid */

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
    for(i=0; i<10000; i++) {
        if ( (user = (example_user_t*)malloc(sizeof(example_user_t))) == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }

    printf("pid: %u\n", (unsigned)getpid());
    /* printf("sig: %p\n", &users->hh.tbl->signature); */
    /* printf("bbv: %p\n", &users->hh.tbl->bloom_bv); */
    sleep(60*10);
    return 0;
}
