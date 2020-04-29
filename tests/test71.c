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

    /* test LL macros */
    printf("LL prepend elem\n");
    LL_APPEND(headA,&els[0]);
    LL_APPEND(headA,&els[1]);
    LL_APPEND(headA,&els[2]);
    LL_APPEND(headA,&els[3]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend head elem */
    LL_PREPEND_ELEM(headA, &els[0], &els[4]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_PREPEND_ELEM(headA, &els[4], &els[5]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend last elem */
    LL_PREPEND_ELEM(headA, &els[3], &els[6]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_PREPEND_ELEM(headA, &els[6], &els[7]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend middle elem */
    LL_PREPEND_ELEM(headA, &els[2], &els[8]);
    LL_PREPEND_ELEM(headA, &els[2], &els[9]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* prepend all just to be sure the list is intact... */
    i = 10;
    LL_FOREACH_SAFE(headA, e, tmp) {
        LL_PREPEND_ELEM(headA, e, &els[i]);
        i++;
    }
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* single elem */
    LL_APPEND(headB, &els[20]);
    LL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_PREPEND_ELEM(headB, &els[20], &els[21]);
    LL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_PREPEND_ELEM(headB, &els[21], &els[22]);
    LL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
