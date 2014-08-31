#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

el *head = NULL;

int eltcmp(el *a, el *b) {return a->id - b->id;}

int main(int argc, char *argv[]) {
    int i;
    el els[10], *e, *tmp, *tmp2;
    for(i=0;i<10;i++) els[i].id='a'+i;

    /* test LL macros */
    printf("LL macros\n");
    LL_APPEND(head,&els[0]);
    LL_APPEND(head,&els[1]);
    LL_APPEND(head,&els[2]);
    LL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    LL_SEARCH_SCALAR(head, e, id, 'b');
    if (e) printf("search scalar found b\n");
    LL_SEARCH(head, e, &els[0], eltcmp);
    if (e) printf("search found %c\n",e->id);
    LL_FOREACH_SAFE(head,e,tmp) LL_DELETE(head,e);

    printf("\n");

    /* test DL macros */
    printf("DL macros\n");
    DL_APPEND(head,&els[0]);
    DL_APPEND(head,&els[1]);
    DL_APPEND(head,&els[2]);
    DL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    DL_SEARCH_SCALAR(head, e, id, 'b');
    if (e) printf("search scalar found b\n");
    DL_SEARCH(head, e, &els[0], eltcmp);
    if (e) printf("search found %c\n",e->id);
    DL_FOREACH_SAFE(head,e,tmp) DL_DELETE(head,e);
    printf("\n");

    /* test CDL macros */
    printf("CDL macros\n");
    CDL_PREPEND(head,&els[0]);
    CDL_PREPEND(head,&els[1]);
    CDL_PREPEND(head,&els[2]);
    CDL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    CDL_SEARCH_SCALAR(head, e, id, 'b');
    if (e) printf("search scalar found b\n");
    CDL_SEARCH(head, e, &els[0], eltcmp);
    if (e) printf("search found %c\n",e->id);
    CDL_FOREACH_SAFE(head,e,tmp,tmp2) CDL_DELETE(head,e);


    return 0;
}
