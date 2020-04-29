#ifdef HASH_FUNCTION
#undef HASH_FUNCTION  /* this test's output depends on the pattern of hash collisions */
#endif

#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

/* This is mostly a copy of test6.c. */

#undef uthash_memcmp
#undef uthash_strlen
#define uthash_memcmp(a,b,n) alt_memcmp(a,b,n)
#define uthash_strlen(s) alt_strlen(s)

typedef struct example_user_t {
    char id[3];
    int cookie;
    UT_hash_handle hh;
} example_user_t;

static int alt_memcmp(const void *a, const void *b, size_t n)
{
    puts("alt_memcmp");
    return memcmp(a,b,n);
}

static size_t alt_strlen(const char *s)
{
    puts("alt_strlen");
    return strlen(s);
}

int main()
{
    int i;
    example_user_t *user, *tmp, *users=NULL;

    /* create elements */
    for (i=0; i<10; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        sprintf(user->id, "%d", i);
        user->cookie = i*i;
        HASH_ADD_STR(users,id,user);
    }

    /* delete each ID */
    for (i=0; i<10; i++) {
        char buffer[3];
        sprintf(buffer, "%d", i);
        HASH_FIND_STR(users,buffer,tmp);
        if (tmp != NULL) {
            HASH_DEL(users,tmp);
            free(tmp);
        } else {
            printf("user id %d not found\n", i);
        }
    }

    /* show the hash */
    for (user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %s, cookie %d\n", user->id, user->cookie);
    }
    return 0;
}
