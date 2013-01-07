#include <stdlib.h>
#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

el *headA, *headB = NULL;

int main(int argc, char *argv[]) {
    int i;
    el els[20], *e, *tmp, *tmp2;
    for(i=0;i<20;i++) els[i].id='a'+i;

    /* test CDL macros */
    printf("CDL replace elem\n");
    CDL_PREPEND(headA,&els[3]);
    CDL_PREPEND(headA,&els[2]);
    CDL_PREPEND(headA,&els[1]);
    CDL_PREPEND(headA,&els[0]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* replace head elem */
    CDL_REPLACE_ELEM(headA, &els[0], &els[4]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");
    CDL_REPLACE_ELEM(headA, &els[4], &els[5]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* replace last elem */
    CDL_REPLACE_ELEM(headA, &els[3], &els[6]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");
    CDL_REPLACE_ELEM(headA, &els[6], &els[7]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* replace middle elem */
    CDL_REPLACE_ELEM(headA, &els[1], &els[8]);
    CDL_REPLACE_ELEM(headA, &els[2], &els[9]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* replace all just to be sure the list is intact... */
    i = 10;
    CDL_FOREACH_SAFE(headA, e, tmp, tmp2) {
        CDL_REPLACE_ELEM(headA, e, &els[i]);
        i++;
    }
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* single elem */
    CDL_PREPEND(headB, &els[18]);
    CDL_FOREACH(headB,e) printf("%c ", e->id); printf("\n");
    CDL_REPLACE_ELEM(headB, &els[18], &els[19]);
    CDL_FOREACH(headB,e) printf("%c ", e->id); printf("\n");

    return 0;
}
