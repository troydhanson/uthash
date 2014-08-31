#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

el *head = NULL;

int main(int argc, char *argv[]) {
    int i;
    el els[10], *e, *tmp, *tmp2;
    for(i=0;i<10;i++) els[i].id='a'+i;

    /* test CDL macros */
    printf("CDL macros\n");
    CDL_PREPEND(head,&els[0]);
    CDL_PREPEND(head,&els[1]);
    CDL_PREPEND(head,&els[2]);
    CDL_FOREACH(head,e) printf("%c ", e->id);

    /* point head to head->next */
    CDL_FOREACH_SAFE(head,e,tmp,tmp2) {
        printf("deleting %c ", e->id);
        CDL_DELETE(head,e);
    }
    printf("\n");
    if (head) printf("non-null head\n");

    /* test DL macros */
    printf("DL macros\n");
    DL_APPEND(head,&els[0]);
    DL_APPEND(head,&els[1]);
    DL_APPEND(head,&els[2]);
    DL_FOREACH(head,e) printf("%c ", e->id);
    DL_FOREACH_SAFE(head,e,tmp) {
        printf("deleting %c ", e->id);
        DL_DELETE(head,e);
    }
    printf("\n");
    if (head) printf("non-null head\n");

    /* test LL macros */
    printf("LL macros\n");
    LL_APPEND(head,&els[0]);
    LL_APPEND(head,&els[1]);
    LL_APPEND(head,&els[2]);
    LL_FOREACH(head,e) printf("%c ", e->id);
    LL_FOREACH_SAFE(head,e,tmp) {
        printf("deleting %c ", e->id);
        LL_DELETE(head,e);
    }
    printf("\n");
    if (head) printf("non-null head\n");

    return 0;
}
