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
    el els[26], *e, *tmp;
    el *headA = NULL;
    el *headB = NULL;
    for(i=0; i<25; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test DL macros */
    printf("DL prepend elem\n");
    DL_APPEND(headA,&els[0]);
    DL_APPEND(headA,&els[1]);
    DL_APPEND(headA,&els[2]);
    DL_APPEND(headA,&els[3]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend head elem */
    DL_PREPEND_ELEM(headA, &els[0], &els[4]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_PREPEND_ELEM(headA, &els[4], &els[5]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend last elem */
    DL_PREPEND_ELEM(headA, &els[3], &els[6]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_PREPEND_ELEM(headA, &els[6], &els[7]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend middle elem */
    DL_PREPEND_ELEM(headA, &els[2], &els[8]);
    DL_PREPEND_ELEM(headA, &els[2], &els[9]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend all just to be sure the list is intact... */
    i = 10;
    DL_FOREACH_SAFE(headA, e, tmp) {
        DL_PREPEND_ELEM(headA, e, &els[i]);
        i++;
    }
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* single elem */
    DL_APPEND(headB, &els[20]);
    DL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_PREPEND_ELEM(headB, &els[20], &els[21]);
    DL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_PREPEND_ELEM(headB, &els[21], &els[22]);
    DL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
