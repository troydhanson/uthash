#include "utlist.h"
#include <assert.h>
#include <stdlib.h>

struct item {
    int x;
    struct item *prev;
    struct item *next;
};

static int sort_cmp(void *a, void *b)
{
    struct item *item1 = (struct item *)a;
    struct item *item2 = (struct item *)b;
    return item1->x - item2->x;
}

void sort_them(struct item **items)
{
    DL_SORT(*items, sort_cmp);
}

int main()
{
    struct item *list = NULL;
    struct item a = { 2, NULL, NULL };
    struct item b = { 1, NULL, NULL };
    sort_them(&list);
    assert(list == NULL);

    DL_APPEND(list, &a);
    assert(list == &a);
    assert(a.prev == &a);
    assert(a.next == NULL);
    sort_them(&list);
    assert(list == &a);
    assert(a.prev == &a);
    assert(a.next == NULL);

    DL_APPEND(list, &b);
    assert(list == &a);
    assert(a.prev == &b);
    assert(a.next == &b);
    assert(b.prev == &a);
    assert(b.next == NULL);
    sort_them(&list);
    assert(list == &b);
    assert(b.prev == &a);
    assert(b.next == &a);
    assert(a.prev == &b);
    assert(a.next == NULL);

    return 0;
}
