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

static void dump_els(struct List *els, int n) {
    printf("---\n");
    for (int i=0; i < n; ++i) {
        printf("%d (prev %d next %d)\n", els[i].value,
            els[i].prev ? els[i].prev->value : -1,
            els[i].next ? els[i].next->value : -1);
    }
}

static void test_ll_prepend() {
    struct List els[] = {
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    printf("test_ll_prepend\n");

    for (i = 0; i < 3; ++i) {
        LL_PREPEND(list, &els[i]);
        dump_els(els, 3);
    }
}

static void test_ll_append() {
    struct List els[] = {
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    printf("test_ll_append\n");

    for (i = 0; i < 3; ++i) {
        LL_APPEND(list, &els[i]);
        dump_els(els, 3);
    }
}

static void test_ll_prepend_elem() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy}, // "non-existent"
    };
    struct List *list = NULL;
    struct List *zeroptr = NULL;

    printf("test_ll_prepend_elem\n");

    // Empty list, prepend before NULL (which means append)
    LL_PREPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0
    dump_els(els, 5);

    // Size-1 list, prepend before head
    LL_PREPEND_ELEM(list, &els[0], &els[1]);
    assert(list == &els[1]); // 1 0
    dump_els(els, 5);

    // Remove an element
    LL_DELETE(list, list);
    assert(list == &els[0]); // 0

    // Size-1 list, prepend before NULL (which means append)
    LL_PREPEND_ELEM(list, zeroptr, &els[1]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Size-2 list, prepend before head
    LL_PREPEND_ELEM(list, &els[0], &els[2]);
    assert(list == &els[2]); // 2 0 1
    dump_els(els, 5);

    // Remove an element
    LL_DELETE(list, list);
    assert(list == &els[0]); // 0 1

    // Size-2 list, prepend before tail
    LL_PREPEND_ELEM(list, &els[1], &els[2]);
    assert(list == &els[0]); // 0 2 1
    dump_els(els, 5);

    // Size-3 list, prepend before NULL (which means append)
    LL_PREPEND_ELEM(list, zeroptr, &els[3]);
    assert(list == &els[0]); // 0 2 1 3
    dump_els(els, 5);
}

static void test_ll_append_elem() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy}, // "non-existent"
    };
    struct List *list = NULL;
    struct List *zeroptr = NULL;

    printf("test_ll_append_elem\n");

    // Empty list, append after NULL (which means prepend)
    LL_APPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0
    dump_els(els, 5);

    // Size-1 list, append after head
    LL_APPEND_ELEM(list, &els[0], &els[1]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Remove an element
    LL_DELETE(list, list);
    assert(list == &els[1]); // 1

    // Size-1 list, append after NULL (which means prepend)
    LL_APPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Size-2 list, append after head
    LL_APPEND_ELEM(list, &els[0], &els[2]);
    assert(list == &els[0]); // 0 2 1
    dump_els(els, 5);

    // Remove an element
    LL_DELETE(list, list);
    assert(list == &els[2]); // 2 1

    // Size-2 list, append after tail
    LL_APPEND_ELEM(list, &els[1], &els[0]);
    assert(list == &els[2]); // 2 1 0
    dump_els(els, 5);

    // Size-3 list, append after NULL (which means prepend)
    LL_APPEND_ELEM(list, zeroptr, &els[3]);
    assert(list == &els[3]); // 3 2 1 0
    dump_els(els, 5);
}

static void test_dl_prepend() {
    struct List els[] = {
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    printf("test_dl_prepend\n");

    for (i = 0; i < 3; ++i) {
        DL_PREPEND(list, &els[i]);
        dump_els(els, 3);
    }
}

static void test_dl_append() {
    struct List els[] = {
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    printf("test_dl_append\n");

    for (i = 0; i < 3; ++i) {
        DL_APPEND(list, &els[i]);
        dump_els(els, 3);
    }
}

static void test_dl_prepend_elem() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy}, // "non-existent"
    };
    struct List *list = NULL;
    struct List *zeroptr = NULL;

    printf("test_dl_prepend_elem\n");

    // Empty list, prepend before NULL (which means append)
    DL_PREPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0
    dump_els(els, 5);

    // Size-1 list, prepend before head
    DL_PREPEND_ELEM(list, &els[0], &els[1]);
    assert(list == &els[1]); // 1 0
    dump_els(els, 5);

    // Remove an element
    DL_DELETE(list, list);
    assert(list == &els[0]); // 0

    // Size-1 list, prepend before NULL (which means append)
    DL_PREPEND_ELEM(list, zeroptr, &els[1]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Size-2 list, prepend before head
    DL_PREPEND_ELEM(list, &els[0], &els[2]);
    assert(list == &els[2]); // 2 0 1
    dump_els(els, 5);

    // Remove an element
    DL_DELETE(list, list);
    assert(list == &els[0]); // 0 1

    // Size-2 list, prepend before tail
    DL_PREPEND_ELEM(list, &els[1], &els[2]);
    assert(list == &els[0]); // 0 2 1
    dump_els(els, 5);

    // Size-3 list, prepend before NULL (which means append)
    DL_PREPEND_ELEM(list, zeroptr, &els[3]);
    assert(list == &els[0]); // 0 2 1 3
    dump_els(els, 5);
}

static void test_dl_append_elem() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy}, // "non-existent"
    };
    struct List *list = NULL;
    struct List *zeroptr = NULL;

    printf("test_dl_append_elem\n");

    // Empty list, append after NULL (which means prepend)
    DL_APPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0
    dump_els(els, 5);

    // Size-1 list, append after head
    DL_APPEND_ELEM(list, &els[0], &els[1]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Remove an element
    DL_DELETE(list, list);
    assert(list == &els[1]); // 1

    // Size-1 list, append after NULL (which means prepend)
    DL_APPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Size-2 list, append after head
    DL_APPEND_ELEM(list, &els[0], &els[2]);
    assert(list == &els[0]); // 0 2 1
    dump_els(els, 5);

    // Remove an element
    DL_DELETE(list, list);
    assert(list == &els[2]); // 2 1

    // Size-2 list, append after tail
    DL_APPEND_ELEM(list, &els[1], &els[0]);
    assert(list == &els[2]); // 2 1 0
    dump_els(els, 5);

    // Size-3 list, append after NULL (which means prepend)
    DL_APPEND_ELEM(list, zeroptr, &els[3]);
    assert(list == &els[3]); // 3 2 1 0
    dump_els(els, 5);
}

static void test_cdl_prepend() {
    struct List els[] = {
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    printf("test_cdl_prepend\n");

    for (i = 0; i < 3; ++i) {
        CDL_PREPEND(list, &els[i]);
        dump_els(els, 3);
    }
}

static void test_cdl_append() {
    struct List els[] = {
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
    };
    struct List *list = NULL;
    int i;

    printf("test_cdl_append\n");

    for (i = 0; i < 3; ++i) {
        CDL_APPEND(list, &els[i]);
        dump_els(els, 3);
    }
}

static void test_cdl_prepend_elem() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy}, // "non-existent"
    };
    struct List *list = NULL;
    struct List *zeroptr = NULL;

    printf("test_cdl_prepend_elem\n");

    // Empty list, prepend before NULL (which means append)
    CDL_PREPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0
    dump_els(els, 5);

    // Size-1 list, prepend before head
    CDL_PREPEND_ELEM(list, &els[0], &els[1]);
    assert(list == &els[1]); // 1 0
    dump_els(els, 5);

    // Remove an element
    CDL_DELETE(list, list);
    assert(list == &els[0]); // 0

    // Size-1 list, prepend before NULL (which means append)
    CDL_PREPEND_ELEM(list, zeroptr, &els[1]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Size-2 list, prepend before head
    CDL_PREPEND_ELEM(list, &els[0], &els[2]);
    assert(list == &els[2]); // 2 0 1
    dump_els(els, 5);

    // Remove an element
    CDL_DELETE(list, list);
    assert(list == &els[0]); // 0 1

    // Size-2 list, prepend before tail
    CDL_PREPEND_ELEM(list, &els[1], &els[2]);
    assert(list == &els[0]); // 0 2 1
    dump_els(els, 5);

    // Size-3 list, prepend before NULL (which means append)
    CDL_PREPEND_ELEM(list, zeroptr, &els[3]);
    assert(list == &els[0]); // 0 2 1 3
    dump_els(els, 5);
}

static void test_cdl_append_elem() {
    struct List els[] = {
        {0, &dummy, &dummy},
        {1, &dummy, &dummy},
        {2, &dummy, &dummy},
        {3, &dummy, &dummy},
        {4, &dummy, &dummy}, // "non-existent"
    };
    struct List *list = NULL;
    struct List *zeroptr = NULL;

    printf("test_cdl_append_elem\n");

    // Empty list, append after NULL (which means prepend)
    CDL_APPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0
    dump_els(els, 5);

    // Size-1 list, append after head
    CDL_APPEND_ELEM(list, &els[0], &els[1]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Remove an element
    CDL_DELETE(list, list);
    assert(list == &els[1]); // 1

    // Size-1 list, append after NULL (which means prepend)
    CDL_APPEND_ELEM(list, zeroptr, &els[0]);
    assert(list == &els[0]); // 0 1
    dump_els(els, 5);

    // Size-2 list, append after head
    CDL_APPEND_ELEM(list, &els[0], &els[2]);
    assert(list == &els[0]); // 0 2 1
    dump_els(els, 5);

    // Remove an element
    CDL_DELETE(list, list);
    assert(list == &els[2]); // 2 1

    // Size-2 list, append after tail
    CDL_APPEND_ELEM(list, &els[1], &els[0]);
    assert(list == &els[2]); // 2 1 0
    dump_els(els, 5);

    // Size-3 list, append after NULL (which means prepend)
    CDL_APPEND_ELEM(list, zeroptr, &els[3]);
    assert(list == &els[3]); // 3 2 1 0
    dump_els(els, 5);
}

int main()
{
    test_ll_prepend();
    test_ll_append();
    test_ll_prepend_elem();
    test_ll_append_elem();

    test_dl_prepend();
    test_dl_append();
    test_dl_prepend_elem();
    test_dl_append_elem();

    test_cdl_prepend();
    test_cdl_append();
    test_cdl_prepend_elem();
    test_cdl_append_elem();
}
