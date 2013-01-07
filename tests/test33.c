#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"

#define BUFLEN 20

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

int namecmp(void *_a, void *_b) {
    el *a = (el*)_a;
    el *b = (el*)_b;
    return strcmp(a->bname,b->bname);
}

el *head = NULL;

int main(int argc, char *argv[]) {
    el *name, *tmp;

    char linebuf[BUFLEN];
    FILE *file;

    if ( (file = fopen( "test11.dat", "r" )) == NULL ) {
        perror("can't open: "); 
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        if ( (name = (el*)malloc(sizeof(el))) == NULL) exit(-1);
        strncpy(name->bname,linebuf,BUFLEN);
        LL_PREPEND(head, name);
    }
    LL_SORT(head, namecmp);
    LL_FOREACH(head,tmp) printf("%s", tmp->bname);

    fclose(file);

    return 0;
}
