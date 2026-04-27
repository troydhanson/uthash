#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utlist.h"

struct List {
    int value;
    struct List *prev;
    struct List *next;
};

static void test_ll_concat(void)
{
    int i;
    struct List els[10], *tmp;
    struct List *list1 = NULL;
    struct List *list2 = NULL;

    for (i = 0; i < 10; i++) {
        els[i].value = (int)'a' + i;
    }

    /* test LL macros */
    printf("test_ll_concat\n");

    LL_APPEND(list1, &els[0]);
    LL_APPEND(list1, &els[1]);
    LL_APPEND(list1, &els[2]);

    i = 0;
    LL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 3);
    printf("\n");

    LL_APPEND(list2, &els[3]);
    LL_APPEND(list2, &els[4]);
    LL_APPEND(list2, &els[5]);

    i = 3;
    LL_FOREACH(list2, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    LL_CONCAT(list1, list2);

    i = 0;
    LL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    list1 = NULL;
    LL_CONCAT(list1, list2);

    assert(list1 == &els[3]);
    LL_FOREACH(list1, tmp) {
        printf("%c ", tmp->value);
    }
    printf("\n");

    list2 = NULL;
    LL_CONCAT(list1, list2);

    i = 3;
    LL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    list1 = NULL;
    list2 = NULL;

    LL_APPEND(list1, &els[0]);
    assert(list1 == &els[0]);

    LL_APPEND(list2, &els[1]);
    assert(list2 == &els[1]);

    LL_CONCAT(list1, list2);

    i = 0;
    LL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 2);
    printf("\n");
}

static void test_dl_concat(void)
{
    int i;
    struct List els[10], *tmp;
    struct List *list1 = NULL;
    struct List *list2 = NULL;

    for (i = 0; i < 10; i++) {
        els[i].value = (int)'a' + i;
    }

    printf("test_dl_concat\n");

    DL_APPEND(list1, &els[0]);
    DL_APPEND(list1, &els[1]);
    DL_APPEND(list1, &els[2]);

    i = 0;
    DL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 3);
    printf("\n");

    DL_APPEND(list2, &els[3]);
    DL_APPEND(list2, &els[4]);
    DL_APPEND(list2, &els[5]);

    i = 3;
    DL_FOREACH(list2, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    DL_CONCAT(list1, list2);

    i = 0;
    DL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    /* other variations */
    list1 = NULL;
    DL_CONCAT(list1, list2);

    assert(list1 == &els[3]);
    DL_FOREACH(list1, tmp) {
        printf("%c ", tmp->value);
    }
    printf("\n");

    list2 = NULL;
    DL_CONCAT(list1, list2);

    i = 3;
    DL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    list1 = NULL;
    list2 = NULL;

    DL_APPEND(list1, &els[0]);
    assert(list1 == &els[0]);

    DL_APPEND(list2, &els[1]);
    assert(list2 == &els[1]);

    DL_CONCAT(list1, list2);

    i = 0;
    DL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 2);
    printf("\n");
}

static void test_cdl_concat(void)
{
    int i;
    struct List els[10], *tmp;
    struct List *list1 = NULL;
    struct List *list2 = NULL;

    for (i = 0; i < 10; i++) {
        els[i].value = (int)'a' + i;
    }

    printf("test_cdl_concat\n");

    CDL_APPEND(list1, &els[0]);
    CDL_APPEND(list1, &els[1]);
    CDL_APPEND(list1, &els[2]);

    i = 0;
    CDL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 3);
    printf("\n");

    CDL_APPEND(list2, &els[3]);
    CDL_APPEND(list2, &els[4]);
    CDL_APPEND(list2, &els[5]);

    i = 3;
    CDL_FOREACH(list2, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    CDL_CONCAT(list1, list2);

    i = 0;
    CDL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    /* other variations */
    list1 = NULL;
    list2 = NULL;

    CDL_APPEND(list2, &els[3]);
    CDL_APPEND(list2, &els[4]);
    CDL_APPEND(list2, &els[5]);

    CDL_CONCAT(list1, list2);

    assert(list1 == &els[3]);
    CDL_FOREACH(list1, tmp) {
        printf("%c ", tmp->value);
    }
    printf("\n");

    list2 = NULL;
    CDL_CONCAT(list1, list2);

    i = 3;
    CDL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 6);
    printf("\n");

    list1 = NULL;
    list2 = NULL;

    CDL_APPEND(list1, &els[0]);
    assert(list1 == &els[0]);

    CDL_APPEND(list2, &els[1]);
    assert(list2 == &els[1]);

    CDL_CONCAT(list1, list2);

    i = 0;
    CDL_FOREACH(list1, tmp) {
        assert(tmp == &els[i]);
        printf("%c ", tmp->value);
        i++;
    }
    assert(i == 2);
    printf("\n");
}

int main(void)
{
    test_ll_concat();
    test_dl_concat();
    test_cdl_concat();
}
