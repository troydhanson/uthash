#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id, score;
    struct el *next2, *prev2;
} el;

static int order_desc(el *a, el *b)
{
    return (a->score > b->score) ? -1 : (a->score < b->score);
}

int main(int argc, char *argv[])
{
    int i;
    el *head = NULL;
    el els[15], *e;

    for (i=0; i<15; i++) {
        els[i].id = (int)'a'+i;
        els[i].score = i%7;
        LL_INSERT_INORDER2(head, &els[i], order_desc, next2);
    }
    LL_FOREACH2(head, e, next2) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("DL_INSERT_INORDER\n");
    head = NULL;
    for (i=0; i<15; i++) {
        DL_INSERT_INORDER2(head, &els[i], order_desc, prev2, next2);
    }
    DL_FOREACH2(head, e, next2) {
        printf("%c ", e->id);
    }
    printf("\n");

    printf("CDL_INSERT_INORDER\n");
    head = NULL;
    for (i=0; i<15; i++) {
        CDL_INSERT_INORDER2(head, &els[i], order_desc, prev2, next2);
    }
    CDL_FOREACH2(head, e, next2) {
        printf("%c ", e->id);
    }
    printf("\n");
    CDL_FOREACH2(head, e, prev2) {
        printf("%c ", e->id);
    }
    printf("\n");
    return 0;
}
