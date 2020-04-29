#include <stdlib.h>
#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

int main()
{
    int i;
    el els[20], *e, *tmp;
    el *headA = NULL;
    el *headB = NULL;
    for(i=0; i<20; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test DL macros */
    printf("DL replace elem\n");
    DL_APPEND(headA,&els[0]);
    DL_APPEND(headA,&els[1]);
    DL_APPEND(headA,&els[2]);
    DL_APPEND(headA,&els[3]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace head elem */
    DL_REPLACE_ELEM(headA, &els[0], &els[4]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_REPLACE_ELEM(headA, &els[4], &els[5]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace last elem */
    DL_REPLACE_ELEM(headA, &els[3], &els[6]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_REPLACE_ELEM(headA, &els[6], &els[7]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace middle elem */
    DL_REPLACE_ELEM(headA, &els[1], &els[8]);
    DL_REPLACE_ELEM(headA, &els[2], &els[9]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace all just to be sure the list is intact... */
    i = 10;
    DL_FOREACH_SAFE(headA, e, tmp) {
        DL_REPLACE_ELEM(headA, e, &els[i]);
        i++;
    }
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* single elem */
    DL_APPEND(headB, &els[18]);
    DL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_REPLACE_ELEM(headB, &els[18], &els[19]);
    DL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
