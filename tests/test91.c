#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id, score;
    struct el *next, *prev;
} el;

static int order_desc(el *a, el *b)
{
    return (a->score > b->score) ? -1 : (a->score < b->score);
}

int main()
{
    int i;
    el *head = NULL;
    el els[15], *e;

    for (i=0; i<15; i++) {
        els[i].id = (int)'a'+i;
        els[i].score = i%7;
        LL_INSERT_INORDER(head, &els[i], order_desc);
    }
    LL_FOREACH(head, e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("DL_INSERT_INORDER\n");
    head = NULL;
    for (i=0; i<15; i++) {
        DL_INSERT_INORDER(head, &els[i], order_desc);
    }
    DL_FOREACH(head, e) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("CDL_INSERT_INORDER\n");
    head = NULL;
    for (i=0; i<15; i++) {
        CDL_INSERT_INORDER(head, &els[i], order_desc);
    }
    CDL_FOREACH(head, e) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_FOREACH2(head, e, prev) {
        printf("%c ", e->id);
    }
    printf("\n");
    return 0;
}
