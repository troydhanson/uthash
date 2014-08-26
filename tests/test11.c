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

int namecmp(void *_a, void *_b) {
    name_rec *a = (name_rec*)_a;
    name_rec *b = (name_rec*)_b;
    return strcmp(a->boy_name,b->boy_name);
}

int main(int argc,char *argv[]) {
    name_rec *name, *names=NULL;
    char linebuf[BUFLEN];
    FILE *file;

    if ( (file = fopen( "test11.dat", "r" )) == NULL ) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        if ( (name = (name_rec*)malloc(sizeof(name_rec))) == NULL) exit(-1);
        strncpy(name->boy_name,linebuf,BUFLEN);
        HASH_ADD_STR(names,boy_name,name);
    }

    fclose(file);
    HASH_SORT(names,namecmp);
    for(name=names;name;name=(name_rec*)(name->hh.next))
      printf("%s",name->boy_name);

   return 0;
}

