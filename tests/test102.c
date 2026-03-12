#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

typedef struct List {
    int value;
    struct List *prev;
    struct List *next;
} List;

int main(void) {
    List *list = NULL, *node, *tmp;
    int i;

    for(i = 1; i <= 1024; i <<= 1) {
        node = (List*)malloc(sizeof(*node));
        node->value = i;
        DL_APPEND(list, node);
    }

    DL_REVERSE(list);

    node = (List*)malloc(sizeof(*node));
    node->value = 0;
    DL_APPEND(list, node);

    node = (List*)malloc(sizeof(*node));
    node->value = 2048;
    DL_PREPEND(list, node);

    DL_FOREACH(list, node) {
        printf("%d\n", node->value);
    }

    for(node = list; node != NULL;) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
}
