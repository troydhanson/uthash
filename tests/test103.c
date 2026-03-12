#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

typedef struct List {
    int value;
    struct List *prev;
    struct List *next;
} List;

int main(void) {
    List *list = NULL, *node,
         *tmp1, *tmp2;
    int i;

    for(i = 1; i <= 1024; i <<= 1) {
        node = (List*)malloc(sizeof(*node));
        node->value = i;
        CDL_APPEND(list, node);
    }

    CDL_REVERSE(list);

    node = (List*)malloc(sizeof(*node));
    node->value = 0;
    CDL_APPEND(list, node);

    node = (List*)malloc(sizeof(*node));
    node->value = 2048;
    CDL_PREPEND(list, node);

    CDL_FOREACH(list, node) {
        printf("%d\n", node->value);
    }

    CDL_FOREACH_SAFE(list, node, tmp1, tmp2) {
        CDL_DELETE(list, node);
        free(node);
    }
}
