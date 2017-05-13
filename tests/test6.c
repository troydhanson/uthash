#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Set up macros for alternative malloc/free functions  */
#undef uthash_malloc
#undef uthash_free
#undef uthash_memcmp
#undef uthash_strlen
#undef uthash_bzero
#define uthash_malloc(sz) alt_malloc(sz)
#define uthash_free(ptr,sz) alt_free(ptr)
#define uthash_memcmp(a,b,n) alt_memcmp(a,b,n)
#define uthash_strlen(s) ..fail_to_compile..
#define uthash_bzero(a,n) alt_bzero(a,n)

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

static void *alt_malloc(size_t sz)
{
    if (sz == sizeof(UT_hash_table)) {
        printf("%s\n", "alt malloc table");
    }
    return malloc(sz);
}
static void alt_free(void *ptr)
{
    /* printf("%s\n", "alt_free"); */
    free(ptr);
}

static int alt_memcmp(void *a, void *b, size_t n)
{
    puts("alt_memcmp");
    return memcmp(a,b,n);
}

static void alt_bzero(void *a, size_t n)
{
    puts("alt_bzero");
    memset(a,0,n);
}

int main(int argc,char *argv[])
{
    int i;
    example_user_t *user, *tmp, *users=NULL;

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

    /* delete each ID */
    for(i=0; i<10; i++) {
        HASH_FIND_INT(users,&i,tmp);
        if (tmp != NULL) {
            HASH_DEL(users,tmp);
            free(tmp);
        } else {
            printf("user id %d not found\n", i);
        }
    }

    /* show the hash */
    for(user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }
    return 0;
}
