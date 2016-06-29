#include <stdio.h>
#include "uthash.h"

typedef struct {
    UT_hash_handle hh;
    char name[32];
    int weight;
} hstruct_t;

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*(x)))
hstruct_t tst[] = {
    {{0}, "muh1", 2},
    {{0}, "muh2", 8},
    {{0}, "muh3", 1},
    {{0}, "muh4", 8},
    {{0}, "muh5", 3},
    {{0}, "muh6", 5},
    {{0}, "muh7", 6},
    {{0}, "muh8", 15},
    {{0}, "muh9", 6},
    {{0}, "muh10", 9},
    {{0}, "muh11", 10},
    {{0}, "muh12", 43}
};
hstruct_t *hTable;

static int cmpfunc(const hstruct_t *s1, const hstruct_t *s2)
{
    return (s1->weight < s2->weight) ? -1 : (s1->weight > s2->weight);
}

void printtable(void)
{
    hstruct_t *search;

    for(search = hTable; search; search = search->hh.next) {
        printf("%d: %s\n", search->weight, search->name);
    }

    printf("###\n");
}

void delitem(const char *name)
{
    hstruct_t *item;

    HASH_FIND_STR(hTable, name, item);
    HASH_DEL(hTable, item);
    printtable();
}

int main()
{
    int index;

    for(index = 0; index < ARRAY_LEN(tst); index++) {
        HASH_SADD(hh, hTable, name[0], strlen(tst[index].name), &tst[index], cmpfunc);
    }

    printtable();
    HASH_SORT(hTable, cmpfunc);
    printtable();

    delitem("muh12");
    delitem("muh1");
    delitem("muh7");
    delitem("muh3");
    delitem("muh9");
    delitem("muh2");
    delitem("muh11");
    delitem("muh4");
    delitem("muh6");
    delitem("muh5");
    delitem("muh8");
    delitem("muh10");

    return 0;
}
