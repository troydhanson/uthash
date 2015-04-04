#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"

#define BUFLEN 20

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

static int namecmp(el *a, el *b) {
    return strcmp(a->bname,b->bname);
}

int main(int argc, char *argv[]) {
    el *name, *tmp;
    el *head = NULL;

    char linebuf[BUFLEN];
    FILE *file;

    if ( (file = fopen( "test11.dat", "r" )) == NULL ) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        if ( (name = (el*)malloc(sizeof(el))) == NULL) exit(-1);
        strncpy(name->bname,linebuf,sizeof(name->bname));
        DL_PREPEND(head, name);
    }
    /* DL_SORT(head, namecmp); */
    DL_FOREACH(head,tmp) printf("%s", tmp->bname);

    fclose(file);

    return 0;
}
