#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

int main(void)
{
    int i;
    el els[10], *e;
    el *headA = NULL;
    el *headB = NULL;
    for(i=0; i<10; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test DL macros */
    printf("DL macros\n");
    DL_APPEND(headA,&els[0]);
    DL_APPEND(headA,&els[1]);
    DL_APPEND(headA,&els[2]);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    DL_APPEND(headB,&els[3]);
    DL_APPEND(headB,&els[4]);
    DL_APPEND(headB,&els[5]);
    DL_FOREACH(headB,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    DL_CONCAT(headA,headB);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* other variations */
    headA = NULL;
    DL_CONCAT(headA,headB);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    headB = NULL;
    DL_CONCAT(headA,headB);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    headA=NULL;
    headB=NULL;
    DL_APPEND(headA,&els[0]);
    DL_APPEND(headB,&els[1]);
    DL_CONCAT(headA,headB);
    DL_FOREACH(headA,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
