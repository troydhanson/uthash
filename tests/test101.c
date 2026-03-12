#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

typedef struct List {
    int value;
    struct List *next;
} List;

int main(void) {
    List *list = NULL, *node, *tmp;
    int i;

    for(i = 1; i <= 1024; i <<= 1) {
        node = malloc(sizeof(*node));
        node->value = i;
        LL_PREPEND(list, node);
    }

    LL_REVERSE(list);

    node = malloc(sizeof(*node));
    node->value = 0;
    LL_PREPEND(list, node);

    node = malloc(sizeof(*node));
    node->value = 2048;
    LL_APPEND(list, node);

    LL_FOREACH(list, node) {
        printf("%d\n", node->value);
    }

    for(node = list; node != NULL;) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
}
