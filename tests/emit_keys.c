#include <stdlib.h>   /* malloc */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */
#include <unistd.h>   /* write */

/* this define must precede uthash.h */
#define HASH_EMIT_KEYS 1
#include "uthash.h"

#define BUFLEN 30

typedef struct name_rec {
    char boy_name[BUFLEN];
    UT_hash_handle hh;
} name_rec;

int main(int argc,char *argv[]) {
    name_rec *name, *names=NULL;
    char linebuf[BUFLEN];
    FILE *file;
    int i=0;

    if (argc != 2) {
        fprintf(stderr,"usage: %s file\n", argv[0]); 
        exit(-1);
    }

    if ( (file = fopen( argv[1], "r" )) == NULL ) {
        perror("can't open: "); 
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        if ( (name = (name_rec*)malloc(sizeof(name_rec))) == NULL) exit(-1);
        strncpy(name->boy_name,linebuf,BUFLEN);
        HASH_ADD_STR(names,boy_name,name);
        i++;
    }

    fprintf(stderr,"%d keys emitted.\n", i);
    fclose(file);
    return 0;
}

