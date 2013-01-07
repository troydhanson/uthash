#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

el *headA = NULL, *headB = NULL;

int main(int argc, char *argv[]) {
    int i;
    el els[10], *e;
    for(i=0;i<10;i++) els[i].id='a'+i;

    /* test LL macros */
    printf("LL macros\n");
    LL_APPEND(headA,&els[0]);
    LL_APPEND(headA,&els[1]);
    LL_APPEND(headA,&els[2]);
    LL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    LL_APPEND(headB,&els[3]);
    LL_APPEND(headB,&els[4]);
    LL_APPEND(headB,&els[5]);
    LL_FOREACH(headB,e) printf("%c ", e->id); printf("\n");

    LL_CONCAT(headA,headB);
    LL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* other variations */
    headA = NULL;
    LL_CONCAT(headA,headB);
    LL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");
    headB = NULL;
    LL_CONCAT(headA,headB);
    LL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    headA=NULL; 
    headB=NULL;
    LL_APPEND(headA,&els[0]);
    LL_APPEND(headB,&els[1]);
    LL_CONCAT(headA,headB);
    LL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    return 0;
}
