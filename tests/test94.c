#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id, score;
    struct el *next, *prev;
    struct el *next_list2, *prev_list2;
} el;

static int order_desc(el *a, el *b)
{
    return (a->score > b->score) ? -1 : (a->score < b->score);
}

static int order_asc(el *a, el *b)
{
    return -order_desc(a, b);
}

int main()
{
    int i;
    el *head = NULL;
    el *head2 = NULL;
    el els[15], *e;

    for (i=0; i<15; i++) {
        els[i].id = (int)'a'+i;
        els[i].score = i%7;
        LL_INSERT_INORDER(head, &els[i], order_desc);
        LL_INSERT_INORDER2(head2, &els[i], order_asc, next_list2);
    }

    printf("LL_INSERT_INORDER\n");
    printf("list1: ");
    LL_FOREACH(head, e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("list2: ");
    LL_FOREACH2(head2, e, next_list2) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("DL_INSERT_INORDER\n");
    head = NULL;
    head2 = NULL;
    for (i=0; i<15; i++) {
        DL_INSERT_INORDER(head, &els[i], order_desc);
        DL_INSERT_INORDER2(head2, &els[i], order_asc, prev_list2, next_list2);
    }

    printf("list1: ");
    DL_FOREACH(head, e) {
        printf("%c ", e->id);
    }
    printf("\n");
    printf("list2: ");
    DL_FOREACH2(head2, e, next_list2) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("CDL_INSERT_INORDER\n");
    head = NULL;
    head2 = NULL;
    for (i=0; i<15; i++) {
        CDL_INSERT_INORDER(head, &els[i], order_desc);
        CDL_INSERT_INORDER2(head2, &els[i], order_asc, prev_list2, next_list2);
    }
    printf("list1:\n");
    CDL_FOREACH(head, e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_FOREACH2(head, e, prev) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("list2:\n");
    CDL_FOREACH2(head2, e, next_list2) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_FOREACH2(head2, e, prev_list2) {
        printf("%c ", e->id);
    }
    printf("\n");

    return 0;
}
