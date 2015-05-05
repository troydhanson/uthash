#include <stdlib.h>
#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

int main(int argc, char *argv[])
{
    int i;
    el els[26], *e, *tmp, *tmp2;
    el *headA = NULL;
    el *headB = NULL;
    for(i=0; i<25; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test CDL macros */
    printf("CDL prepend elem\n");
    CDL_PREPEND(headA,&els[3]);
    CDL_PREPEND(headA,&els[2]);
    CDL_PREPEND(headA,&els[1]);
    CDL_PREPEND(headA,&els[0]);
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend head elem */
    CDL_PREPEND_ELEM(headA, &els[0], &els[4]);
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_PREPEND_ELEM(headA, &els[4], &els[5]);
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend last elem */
    CDL_PREPEND_ELEM(headA, &els[3], &els[6]);
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_PREPEND_ELEM(headA, &els[6], &els[7]);
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend middle elem */
    CDL_PREPEND_ELEM(headA, &els[2], &els[8]);
    CDL_PREPEND_ELEM(headA, &els[2], &els[9]);
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend all just to be sure the list is intact... */
    i = 10;
    CDL_FOREACH_SAFE(headA, e, tmp, tmp2) {
        CDL_PREPEND_ELEM(headA, e, &els[i]);
        i++;
    }
    CDL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* single elem */
    CDL_PREPEND(headB, &els[20]);
    CDL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_PREPEND_ELEM(headB, &els[20], &els[21]);
    CDL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_PREPEND_ELEM(headB, &els[21], &els[22]);
    CDL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
