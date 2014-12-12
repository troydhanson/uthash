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

    /* test DL macros */
    printf("DL macros\n");
    CDL_APPEND(headA,&els[0]);
    CDL_APPEND(headA,&els[1]);
    CDL_APPEND(headA,&els[2]);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    CDL_APPEND(headB,&els[3]);
    CDL_APPEND(headB,&els[4]);
    CDL_APPEND(headB,&els[5]);
    CDL_FOREACH(headB,e) printf("%c ", e->id); printf("\n");

    CDL_CONCAT(headA,headB);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    /* other variations */
    headA = NULL;
    headB = NULL;
    CDL_APPEND(headB,&els[3]);
    CDL_APPEND(headB,&els[4]);
    CDL_APPEND(headB,&els[5]);
    
    CDL_CONCAT(headA,headB);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");
    
    headA = NULL;
    headB = NULL;
    CDL_APPEND(headA,&els[0]);
    CDL_APPEND(headA,&els[1]);
    CDL_APPEND(headA,&els[2]);
    CDL_CONCAT(headA,headB);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    headA=NULL;
    headB=NULL;
    CDL_APPEND(headA,&els[0]);
    CDL_APPEND(headB,&els[1]);
    CDL_CONCAT(headA,headB);
    CDL_FOREACH(headA,e) printf("%c ", e->id); printf("\n");

    return 0;
}