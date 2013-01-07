#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *Next, *Prev;
} el;

el *head = NULL;

int main(int argc, char *argv[]) {
    int i;
    el els[10], *e;
    for(i=0;i<10;i++) els[i].id='a'+i;

    /* test CDL macros */
    printf("CDL macros\n");
    CDL_PREPEND2(head,&els[0],Prev,Next);
    CDL_PREPEND2(head,&els[1],Prev,Next);
    CDL_PREPEND2(head,&els[2],Prev,Next);
    CDL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    /* point head to head->next */
    printf("advancing head pointer\n");
    head = head->Next;
    CDL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    /* follow circular loop a few times */
    for(i=0,e=head;e && i<10;i++,e=e->Next) 
        printf("%c ", e->id); 
    printf("\n");

    /* follow circular loop backwards a few times */
    for(i=0,e=head;e && i<10;i++,e=e->Prev) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting b\n");
    CDL_DELETE2(head,&els[1],Prev,Next);
    CDL_FOREACH2(head,e,Next) printf("%c ", e->id); 
    printf("\n");
    printf("deleting (a)\n");
    CDL_DELETE2(head,&els[0],Prev,Next);
    CDL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");
    printf("deleting (c)\n");
    CDL_DELETE2(head,&els[2],Prev,Next);
    CDL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    /* test DL macros */
    printf("DL macros\n");
    DL_APPEND2(head,&els[0],Prev,Next);
    DL_APPEND2(head,&els[1],Prev,Next);
    DL_APPEND2(head,&els[2],Prev,Next);
    DL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting tail c\n");
    DL_DELETE2(head,&els[2],Prev,Next);
    DL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting head a\n");
    DL_DELETE2(head,&els[0],Prev,Next);
    DL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting head b\n");
    DL_DELETE2(head,&els[1],Prev,Next);
    DL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    /* test LL macros */
    printf("LL macros\n");
    LL_APPEND2(head,&els[0],Next);
    LL_APPEND2(head,&els[1],Next);
    LL_APPEND2(head,&els[2],Next);
    LL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting tail c\n");
    LL_DELETE2(head,&els[2],Next);
    LL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting head a\n");
    LL_DELETE2(head,&els[0],Next);
    LL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    printf("deleting head b\n");
    LL_DELETE2(head,&els[1],Next);
    LL_FOREACH2(head,e,Next) 
        printf("%c ", e->id); 
    printf("\n");

    return 0;
}
