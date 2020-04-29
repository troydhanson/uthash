#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */

#define BUFLEN 20

#if 0
/* Print a message if the hash's no-expand flag is set. */
#undef uthash_noexpand_fyi
#undef uthash_expand_fyi
#define uthash_noexpand_fyi(tbl) printf("noexpand set\n");
#define uthash_expand_fyi(tbl) printf("hash expanded\n");
#endif

typedef struct name_rec {
    char boy_name[BUFLEN];
    UT_hash_handle hh;
} name_rec;

static int namecmp(void *_a, void *_b)
{
    name_rec *a = (name_rec*)_a;
    name_rec *b = (name_rec*)_b;
    return strcmp(a->boy_name,b->boy_name);
}

int main(void)
{
    name_rec *name, *names=NULL;
    char linebuf[BUFLEN];
    FILE *file;

    file = fopen( "test11.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        name = (name_rec*)malloc(sizeof(name_rec));
        if (name == NULL) {
            exit(-1);
        }
        strcpy(name->boy_name, linebuf);
        HASH_ADD_STR(names,boy_name,name);
    }

    fclose(file);
    HASH_SORT(names,namecmp);
    for(name=names; name!=NULL; name=(name_rec*)(name->hh.next)) {
        printf("%s",name->boy_name);
    }

    return 0;
}

