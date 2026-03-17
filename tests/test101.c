#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

typedef struct List {
    int value;
    struct List *next;
} List;

int main() {
    List *list = NULL, *node, *tmp;
    int i;

    LL_REVERSE(list);
    assert(list == NULL);

    for (i = 0; i < 10; ++i) {
        node = (List*)malloc(sizeof(*node));
        node->value = i;
        LL_REVERSE(list);
        LL_PREPEND(list, node);
        LL_FOREACH(list, node) {
            printf("%d", node->value);
        }
        printf("\n");
    }
    LL_FOREACH_SAFE(list, node, tmp) {
        LL_DELETE(list, node);
        free(node);
    }
}
