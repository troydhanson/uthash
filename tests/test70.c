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
    el els[20], *e, *tmp;
    el *headA = NULL;
    el *headB = NULL;
    for(i=0; i<20; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test LL macros */
    printf("LL replace elem\n");
    LL_APPEND(headA,&els[0]);
    LL_APPEND(headA,&els[1]);
    LL_APPEND(headA,&els[2]);
    LL_APPEND(headA,&els[3]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace head elem */
    LL_REPLACE_ELEM(headA, &els[0], &els[4]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_REPLACE_ELEM(headA, &els[4], &els[5]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace last elem */
    LL_REPLACE_ELEM(headA, &els[3], &els[6]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_REPLACE_ELEM(headA, &els[6], &els[7]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace middle elem */
    LL_REPLACE_ELEM(headA, &els[1], &els[8]);
    LL_REPLACE_ELEM(headA, &els[2], &els[9]);
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* replace all just to be sure the list is intact... */
    i = 10;
    LL_FOREACH_SAFE(headA, e, tmp) {
        LL_REPLACE_ELEM(headA, e, &els[i]);
        i++;
    }
    LL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* single elem */
    LL_APPEND(headB, &els[18]);
    LL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_REPLACE_ELEM(headB, &els[18], &els[19]);
    LL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
