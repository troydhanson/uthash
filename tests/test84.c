#define HASH_BLOOM 16
#include "uthash.h"
#include <stdlib.h>   /* malloc */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */

#define BUFLEN 20

typedef struct name_rec {
    char boy_name[BUFLEN];
    UT_hash_handle hh;
} name_rec;

int main(int argc,char *argv[]) {
    name_rec *name, *names=NULL;
    char linebuf[BUFLEN];
    FILE *file;

    if ( (file = fopen( "test84.dat", "r" )) == NULL ) {
        perror("can't open: "); 
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        if ( (name = (name_rec*)malloc(sizeof(name_rec))) == NULL) exit(-1);
        strncpy(name->boy_name,linebuf,BUFLEN);
        HASH_ADD_STR(names,boy_name,name);
        printf("items in hash: %u, overhead: %u\n",(unsigned)HASH_COUNT(names),
          (unsigned)HASH_OVERHEAD(hh,names));
    }

    fclose(file);

   return 0;
}

