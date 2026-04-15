#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

struct list {
    int val;
    struct list *prev, *next;
};

int main(void) {
    int i;
    struct list *list1 = NULL,
                *list2 = NULL,
                *tmp, *tmp1, *tmp2;

    // even list
    for (i = 1; i <= 10; i++) {
        if ((i & 1) == 0) {
            tmp = (struct list*)malloc(sizeof(*tmp));
            tmp->val = i;
            CDL_APPEND(list1, tmp);
        }
    }

    // odd list
    for (i = 1; i <= 10; i++) {
        if (i & 1) {
            tmp = (struct list*)malloc(sizeof(*tmp));
            tmp->val = i;
            CDL_APPEND(list2, tmp);
        }
    }

    CDL_CONCAT(list1, list2);

    CDL_FOREACH(list1, tmp) {
        printf("%d\n", tmp->val) ;
    }

    CDL_FOREACH_SAFE(list1, tmp, tmp1, tmp2) {
        free(tmp);
    }

    return 0;
}
