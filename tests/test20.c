#include <string.h>  /* memcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash.h"

struct my_struct {
    char bkey[5];              /* "binary" key */
    int data;
    UT_hash_handle hh;
};

int main()
{
    struct my_struct *s, *t, *bins = NULL;
    char binary[5] = {'\3','\1','\4','\1','\6'};

    /* allocate our structure. initialize to some values */
    s = (struct my_struct*)calloc(1UL,sizeof(struct my_struct));
    if (s == NULL) {
        exit(-1);
    }
    memcpy(s->bkey, binary, sizeof(binary));

    /* add to hash table using general macro */
    HASH_ADD( hh, bins, bkey, sizeof(binary), s);

    /* look up the structure we just added */
    HASH_FIND( hh, bins, binary, sizeof(binary), t );

    if (t != NULL) {
        printf("found\n");
    }
    return 0;
}
