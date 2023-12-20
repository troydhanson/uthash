#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash.h"

struct my_struct {
    const char *name;          /* key */
    int id;
    UT_hash_handle hh;         /* makes this structure hashable */
};

int main() {
    const char *names[] = { "joe", "bob", "betty", NULL };
    struct my_struct *s, *tmp, *users = NULL;

    for (int i = 0; names[i]; ++i) {
        s = (struct my_struct *)malloc(sizeof *s);
        s->name = names[i];
        s->id = i;
        HASH_ADD_KEYPTR(hh, users, s->name, strlen(s->name), s);
    }

    HASH_FIND_STR(users, "betty", s);
    if (s) printf("betty's id is %d\n", s->id);

    s = (struct my_struct *)malloc(sizeof *s);
    s->name = "bob";
    s->id = 3;
    HASH_REPLACE_KEYPTR(hh, users, s->name, strlen(s->name), s, tmp);
    if (tmp) {
        printf("bob's id was %d, but now it's 3\n", tmp->id);
        free(tmp);
    }

    /* free the hash table contents */
    HASH_ITER(hh, users, s, tmp) {
        HASH_DEL(users, s);
        free(s);
    }
    return 0;
}
