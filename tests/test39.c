#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

typedef struct {
    const char *name;
    UT_hash_handle hh;
} ns_t;

int main()
{
    const char *keys[] = {"eins", "zwei", "drei"};
    unsigned i;
    ns_t *nsp;
    ns_t *head = NULL;

    for(i=0; i < (sizeof(keys)/sizeof(keys[0])); i++) {
        printf("adding key %s\n", keys[i]);
        nsp = (ns_t*)malloc(sizeof(ns_t));
        if (nsp == NULL) {
            exit(-1);
        }
        nsp->name = keys[i];
        HASH_ADD_KEYPTR(hh,head,nsp->name,strlen(nsp->name),nsp);
    }
    printf("hash count is %u\n", HASH_COUNT(head));

    for(i=0; i < (sizeof(keys)/sizeof(keys[0])); i++) {
        printf("looking for key %s... ", keys[i]);
        HASH_FIND(hh,head,keys[i],strlen(keys[i]),nsp);
        printf("%s.\n", (nsp!=NULL)?"found":"not found");
    }
    return 0;
}
