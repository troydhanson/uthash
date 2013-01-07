#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

/* Set up macros for alternative malloc/free functions  */
#undef uthash_malloc
#undef uthash_free
#define uthash_malloc(sz) alt_malloc(sz)
#define uthash_free(ptr,sz) alt_free(ptr)

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

void *alt_malloc(size_t sz) {
    if (sz == sizeof(UT_hash_table)) printf("%s\n", "alt malloc table");
    return malloc(sz);
}
void alt_free(void *ptr) {
    /* printf("%s\n", "alt_free"); */
    free(ptr);
}

int main(int argc,char *argv[]) {
    int i;
    example_user_t *user, *tmp, *users=NULL;

    /* create elements */
    for(i=0;i<10;i++) {
        if ( (user = (example_user_t*)malloc(sizeof(example_user_t))) == NULL) exit(-1);
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }

    /* delete each ID */
    for(i=0;i<10;i++) {
        HASH_FIND_INT(users,&i,tmp);
        if (tmp) {
            HASH_DEL(users,tmp);
            free(tmp);
        } else printf("user id %d not found\n", i);
    }

    /* show the hash */
    for(user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }
   return 0;
}
