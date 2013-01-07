#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
    UT_hash_handle alth;
} example_user_t;

int main(int argc,char *argv[]) {
    int i;
    example_user_t *user, *users=NULL, *altusers=NULL;

    /* create elements */
    for(i=0;i<10;i++) {
        if ( (user = (example_user_t*)malloc(sizeof(example_user_t))) == NULL) exit(-1);
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
        HASH_ADD(alth,altusers,cookie,sizeof(int),user);
    }

    for(user=altusers; user != NULL; user=(example_user_t*)(user->alth.next)) {
        printf("cookie %d, user %d\n", user->cookie, user->id);
    }
   return 0;
}
