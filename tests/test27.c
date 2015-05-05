#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

int main(int argc, char *argv[])
{
    int i;
    el els[10], *e;
    el *head = NULL;
    for(i=0; i<10; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test CDL macros */
    printf("CDL macros\n");
    CDL_PREPEND(head,&els[0]);
    CDL_PREPEND(head,&els[1]);
    CDL_PREPEND(head,&els[2]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* point head to head->next */
    printf("advancing head pointer\n");
    head = head->next;
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* follow circular loop a few times */
    for(i=0,e=head; e && i<10; i++,e=e->next) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* follow circular loop backwards a few times */
    for(i=0,e=head; e && i<10; i++,e=e->prev) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting b\n");
    CDL_DELETE(head,&els[1]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting head (a)\n");
    CDL_DELETE(head,&els[0]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting new head (c)\n");
    CDL_DELETE(head,&els[2]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* test DL macros */
    printf("DL macros\n");
    DL_PREPEND(head,&els[0]);
    DL_PREPEND(head,&els[1]);
    DL_PREPEND(head,&els[2]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting c\n");
    DL_DELETE(head,&els[2]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting a\n");
    DL_DELETE(head,&els[0]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting b\n");
    DL_DELETE(head,&els[1]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* test LL macros */
    printf("LL macros\n");
    LL_PREPEND(head,&els[0]);
    LL_PREPEND(head,&els[1]);
    LL_PREPEND(head,&els[2]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting c\n");
    LL_DELETE(head,&els[2]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting a\n");
    LL_DELETE(head,&els[0]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting b\n");
    LL_DELETE(head,&els[1]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
