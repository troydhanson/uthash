#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

struct List {
    int value;
    struct List *prev;
    struct List *next;
};

struct List dummy = {9, NULL, NULL};

static void dump_els(struct List *list, struct List *els, int n) {
    printf("--- list = &els[%d]\n", (int)(list - els));
    for (int i=0; i < n; ++i) {
        printf("%d (prev %d next %d)\n", els[i].value,
            els[i].prev ? els[i].prev->value : -1,
            els[i].next ? els[i].next->value : -1);
    }
}

static void test_dl_reverse() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    DL_REVERSE(list);
    assert(list == NULL);

    for (i = 0; i < 5; ++i) {
        DL_REVERSE(list);
        DL_PREPEND(list, &els[i]);
        dump_els(list, els, 5);
    }
}

static void test_cdl_reverse() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    CDL_REVERSE(list);
    assert(list == NULL);

    for (i = 0; i < 5; ++i) {
        CDL_REVERSE(list);
        CDL_PREPEND(list, &els[i]);
        dump_els(list, els, 5);
    }
}

int main() {
    test_dl_reverse();
    test_cdl_reverse();
}
