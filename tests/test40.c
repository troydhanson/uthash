#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash.h"

struct my_struct {
    const char *name;          /* key */
    int id;
    UT_hash_handle hh;         /* makes this structure hashable */
};


int main(void)
{
    const char **n, *names[] = { "joe", "bob", "betty", NULL };
    struct my_struct *s, *tmp, *users = NULL;
    int i=0;

    for (n = names; *n != NULL; n++) {
        s = (struct my_struct*)malloc(sizeof(struct my_struct));
        if (s == NULL) {
            exit(-1);
        }
        s->name = *n;
        s->id = i++;
        HASH_ADD_KEYPTR( hh, users, s->name, strlen(s->name), s );
    }

    HASH_FIND_STR( users, "betty", s);
    if (s != NULL) {
        printf("betty's id is %d\n", s->id);
    }

    /* free the hash table contents */
    HASH_ITER(hh, users, s, tmp) {
        HASH_DEL(users, s);
        free(s);
    }
    return 0;
}
