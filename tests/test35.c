#include "uthash.h"
#include <string.h>   /* strcpy */
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct elt {
    char *s;
    UT_hash_handle hh;
} elt;

int main()
{
    int i;
    elt *head = NULL;
    elt elts[10];
    char label[6] = "hello";
    for(i=0; i<10; i++) {
        elts[i].s = (char*)malloc(6UL);
        strcpy(elts[i].s, "hello");
        elts[i].s[0] = 'a' + i;
        printf("%d: %s\n", i, elts[i].s);
        HASH_ADD_KEYPTR(hh, head, elts[i].s, 6UL, &elts[i]);
    }

    /* look up each element and verify the result pointer */
    for(i=0; i<10; i++) {
        elt *e;
        label[0] = 'a' + i;
        HASH_FIND(hh,head,label,6UL,e);
        if (e != NULL) {
            printf( "found %s\n", e->s);
            printf( "right address? %s\n", (e == &elts[i]) ? "yes" : "no");
        }
    }

    return 0;
}
