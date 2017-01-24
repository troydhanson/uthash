#include <assert.h>
#include <stdio.h>
#include "uthash.h"

typedef struct {
    char name[32];
    int weight;
    UT_hash_handle hh;
} hstruct_t;

static int cmpfunc(const hstruct_t *s1, const hstruct_t *s2)
{
    return (s1->weight < s2->weight) ? -1 : (s1->weight > s2->weight);
}

// Test that CMPFUNC can safely be a macro.
#define CMPFUNC(a,b) cmpfunc(a,b)

void printtable(const hstruct_t *hTable)
{
    const hstruct_t *search, *tmp;
    HASH_ITER(hh, hTable, search, tmp) {
        printf("%d: %s\n", search->weight, search->name);
    }
    printf("###\n");
}

void delitem(hstruct_t **hTable, const char *name)
{
    hstruct_t *item;
    HASH_FIND_STR(*hTable, name, item);
    HASH_DEL(*hTable, item);
}

int main()
{
    hstruct_t *hTable = NULL;
    hstruct_t *replaced = NULL;
    unsigned hashvalue;

    hstruct_t tst[] = {
        {"muh1", 2,   {0}},
        {"muh2", 8,   {0}},
        {"muh3", 1,   {0}},
        {"muh4", 8,   {0}},
        {"muh5", 3,   {0}},
        {"muh6", 5,   {0}},
        {"muh7", 6,   {0}},
        {"muh8", 15,  {0}},
        {"muh9", 6,   {0}},
        {"muh10", 9,  {0}},
        {"muh11", 10, {0}},
        {"muh12", 43, {0}},
        {"muh12", 7,  {0}}
    };

    int index;
    for (index = 0; index < 11; ++index) {
        HASH_ADD_INORDER(hh, hTable, name[0], strlen(tst[index].name), &tst[index], CMPFUNC);
    }

    // test HASH_ADD_BYHASHVALUE_INORDER
    HASH_VALUE(tst[11].name, strlen(tst[11].name), hashvalue);
    HASH_ADD_BYHASHVALUE_INORDER(hh, hTable, name[0], strlen(tst[11].name), hashvalue, &tst[11], CMPFUNC);

    printtable(hTable);

    // replace "43: muh12" with "7: muh12"
    HASH_REPLACE_INORDER(hh, hTable, name[0], strlen(tst[11].name), &tst[12], replaced, CMPFUNC);
    assert(replaced == &tst[11]);

    printtable(hTable);

    // rehash "1: muh3" to "9: muh3"
    tst[2].weight = 9;
    HASH_REPLACE_INORDER(hh, hTable, name[0], strlen(tst[2].name), &tst[2], replaced, CMPFUNC);
    assert(replaced == &tst[2]);

    printtable(hTable);

    // rehash "6: muh7" to "16: muh7"
    tst[6].weight = 16;
    HASH_VALUE(&tst[6].name[0], strlen(tst[6].name), hashvalue);
    HASH_REPLACE_BYHASHVALUE_INORDER(hh, hTable, name[0], strlen(tst[6].name), hashvalue, &tst[6], replaced, CMPFUNC);
    assert(replaced == &tst[6]);

    printtable(hTable);

    // remove "8: muh2"...
    HASH_DELETE(hh, hTable, &tst[1]);

    printtable(hTable);

    // ...and then reinsert "8: muh2"
    HASH_VALUE(tst[1].name, strlen(tst[1].name), hashvalue);
    HASH_ADD_KEYPTR_BYHASHVALUE_INORDER(hh, hTable, tst[1].name, strlen(tst[1].name), hashvalue, &tst[1], CMPFUNC);

    printtable(hTable);

    delitem(&hTable, "muh1");
    delitem(&hTable, "muh7");
    delitem(&hTable, "muh3");
    delitem(&hTable, "muh9");
    delitem(&hTable, "muh2");
    delitem(&hTable, "muh11");
    delitem(&hTable, "muh4");
    delitem(&hTable, "muh6");
    delitem(&hTable, "muh5");
    delitem(&hTable, "muh8");
    delitem(&hTable, "muh10");
    delitem(&hTable, "muh12");

    printtable(hTable);

    return 0;
}
