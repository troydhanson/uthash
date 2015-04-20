#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"

#define BUFLEN 20

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

static int namecmp(void *_a, void *_b)
{
    el *a = (el*)_a;
    el *b = (el*)_b;
    return strcmp(a->bname,b->bname);
}

int main(int argc, char *argv[])
{
    el *name, *elt, *tmp, etmp;
    el *head = NULL; /* important- initialize to NULL! */

    char linebuf[BUFLEN];
    FILE *file;

    file = fopen( "test11.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        name = (el*)malloc(sizeof(el));
        if (name == NULL) {
            exit(-1);
        }
        strncpy(name->bname,linebuf,sizeof(name->bname));
        DL_APPEND(head, name);
    }
    DL_SORT(head, namecmp);
    DL_FOREACH(head,elt) {
        printf("%s", elt->bname);
    }

    memcpy(etmp.bname, "WES\n", 5UL);
    DL_SEARCH(head,elt,&etmp,namecmp);
    if (elt != NULL) {
        printf("found %s\n", elt->bname);
    }

    /* now delete each element, use the safe iterator */
    DL_FOREACH_SAFE(head,elt,tmp) {
        DL_DELETE(head,elt);
    }

    fclose(file);

    return 0;
}
