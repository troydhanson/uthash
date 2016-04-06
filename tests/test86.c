#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

int main(int argc, char *argv[])
{
    int i;
    int count;
    el els[10], *e;
    el *head = NULL;
    el *nullptr = NULL;
    for(i=0; i<10; i++) {
        els[i].id=(int)'a'+i;
    }

    /* test CDL macros */
    printf("CDL appends\n");
    CDL_APPEND(head,&els[0]);
    CDL_APPEND(head,&els[1]);
    CDL_APPEND(head,&els[2]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_COUNT(head,e, count);
    printf("count = %d\n", count);

    printf("Test CDL_PREPEND_ELEM %c with elt NULL\n", els[3].id);
    CDL_PREPEND_ELEM(head, nullptr, &els[3]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test CDL_PREPEND_ELEM %c before item %c\n", els[4].id, els[1].id);
    CDL_PREPEND_ELEM(head, &els[1], &els[4]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test CDL_APPEND_ELEM %c with elt NULL\n", els[5].id);
    CDL_APPEND_ELEM(head, nullptr, &els[5]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test CDL_APPEND_ELEM %c after item %c\n", els[6].id, els[1].id);
    CDL_APPEND_ELEM(head, &els[1], &els[6]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_COUNT(head,e, count);
    printf("count = %d\n", count);

    /* point head to head->next */
    printf("advancing head pointer\n");
    head = head->next;
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* follow circular loop a few times */
    for(i=0,e=head; e && i<20; i++,e=e->next) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* follow circular loop backwards a few times */
    for(i=0,e=head; e && i<10; i++,e=e->prev) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("deleting (b)\n");
    CDL_DELETE(head,&els[1]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (a)\n");
    CDL_DELETE(head,&els[0]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (c)\n");
    CDL_DELETE(head,&els[2]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (g)\n");
    CDL_DELETE(head,&els[6]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (e)\n");
    CDL_DELETE(head,&els[4]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (d)\n");
    CDL_DELETE(head,&els[3]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("deleting (f)\n");
    CDL_DELETE(head,&els[5]);
    CDL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    /* test DL macros */
    printf("DL appends\n");

    DL_APPEND(head,&els[0]);
    DL_APPEND(head,&els[1]);
    DL_APPEND(head,&els[2]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_COUNT(head,e, count);
    printf("count = %d\n", count);

    printf("Test DL_PREPEND_ELEM %c with elt NULL\n", els[3].id);
    DL_PREPEND_ELEM(head, nullptr, &els[3]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test DL_PREPEND_ELEM %c before item %c\n", els[4].id, els[1].id);
    DL_PREPEND_ELEM(head, &els[1], &els[4]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test DL_APPEND_ELEM %c with elt NULL\n", els[5].id);
    DL_APPEND_ELEM(head, nullptr, &els[5]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test DL_APPEND_ELEM %c after item %c\n", els[6].id, els[1].id);
    DL_APPEND_ELEM(head, &els[1], &els[6]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    DL_COUNT(head,e, count);
    printf("count = %d\n", count);

    printf("deleting (b)\n");
    DL_DELETE(head,&els[1]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (a)\n");
    DL_DELETE(head,&els[0]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (c)\n");
    DL_DELETE(head,&els[2]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (g)\n");
    DL_DELETE(head,&els[6]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (e)\n");
    DL_DELETE(head,&els[4]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (d)\n");
    DL_DELETE(head,&els[3]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("deleting (f)\n");
    DL_DELETE(head,&els[5]);
    DL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");


    /* test LL macros */
    printf("LL appends\n");

    LL_APPEND(head,&els[0]);
    LL_APPEND(head,&els[1]);
    LL_APPEND(head,&els[2]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_COUNT(head,e, count);
    printf("count = %d\n", count);

    printf("Test LL_PREPEND_ELEM %c with elt NULL\n", els[3].id);
    LL_PREPEND_ELEM(head, nullptr, &els[3]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test LL_PREPEND_ELEM %c before item %c\n", els[4].id, els[1].id);
    LL_PREPEND_ELEM(head, &els[1], &els[4]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test LL_APPEND_ELEM %c with elt NULL\n", els[5].id);
    LL_APPEND_ELEM(head, nullptr, &els[5]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("Test LL_APPEND_ELEM %c after item %c\n", els[6].id, els[1].id);
    LL_APPEND_ELEM(head, &els[1], &els[6]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    LL_COUNT(head,e, count);
    printf("count = %d\n", count);

    printf("deleting (b)\n");
    LL_DELETE(head,&els[1]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (a)\n");
    LL_DELETE(head,&els[0]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (c)\n");
    LL_DELETE(head,&els[2]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (g)\n");
    LL_DELETE(head,&els[6]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (e)\n");
    LL_DELETE(head,&els[4]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("deleting (d)\n");
    LL_DELETE(head,&els[3]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("deleting (f)\n");
    LL_DELETE(head,&els[5]);
    LL_FOREACH(head,e) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
