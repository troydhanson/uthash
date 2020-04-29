#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
    UT_hash_handle alth;
} example_user_t;

int main()
{
    int i;
    example_user_t *user, *tmp, *users=NULL, *altusers=NULL;

    /* create elements */
    for(i=0; i<1000; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        if (i<10) {
            HASH_ADD_INT(users,id,user);
        }
        HASH_ADD(alth,altusers,id,sizeof(int),user);
    }

    /*
    printf("hh items: %d, alth items: %d\n",
            users->hh.tbl->num_items, users->alth.tbl->num_items);
    printf("hh buckets: %d, alth buckets: %d\n",
            users->hh.tbl->num_buckets, users->alth.tbl->num_buckets);
    */

    i=9;
    HASH_FIND_INT(users,&i,tmp);
    printf("%d %s in hh\n", i, (tmp != NULL) ? "found" : "not found");
    HASH_FIND(alth,altusers,&i,sizeof(int),tmp);
    printf("%d %s in alth\n", i, (tmp != NULL) ? "found" : "not found");

    i=10;
    HASH_FIND_INT(users,&i,tmp);
    printf("%d %s in hh\n", i, (tmp != NULL) ? "found" : "not found");
    HASH_FIND(alth,altusers,&i,sizeof(int),tmp);
    printf("%d %s in alth\n", i, (tmp != NULL) ? "found" : "not found");

    return 0;
}
