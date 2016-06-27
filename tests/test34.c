#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"

#define BUFLEN 20

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

int main(int argc, char *argv[])
{
    el *name, *tmp;
    el *head = NULL;

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
        CDL_PREPEND(head, name);
    }
    /* CDL_SORT(head, namecmp); */
    CDL_FOREACH(head,tmp) {
        printf("%s", tmp->bname);
    }

    fclose(file);

    return 0;
}
