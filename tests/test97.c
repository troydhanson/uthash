#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"

struct item {
    int payload;
    UT_hash_handle hh;
};

void delete_without_modifying(struct item *head, const struct item *p)
{
    struct item old;
    memcpy(&old, p, sizeof(struct item)); // also copy the padding bits
    assert(memcmp(&old, p, sizeof(struct item)) == 0);
    assert(p->hh.tbl == head->hh.tbl); // class invariant
    HASH_DEL(head, p);
    assert(memcmp(&old, p, sizeof(struct item)) == 0);  // unmodified by HASH_DEL
}

int main()
{
    struct item *items = NULL;
    struct item *found = NULL;
    int fortytwo = 42;
    int i;

    for (i=0; i < 100; i++) {
        struct item *p = (struct item *)malloc(sizeof *p);
        p->payload = i;
        HASH_ADD_INT(items, payload, p);
    }
    assert(HASH_COUNT(items) == 100);

    // Delete item "42" from the hash, wherever it is.
    HASH_FIND_INT(items, &fortytwo, found);
    assert(found != NULL);
    assert(found->payload == 42);
    delete_without_modifying(items, found);

    assert(HASH_COUNT(items) == 99);
    HASH_FIND_INT(items, &fortytwo, found);
    assert(found == NULL);

    // Delete the very first item in the hash.
    assert(items != NULL);
    i = items->payload;
    delete_without_modifying(items, items);

    assert(HASH_COUNT(items) == 98);
    HASH_FIND_INT(items, &i, found);
    assert(found == NULL);

    // leak the items, we don't care

    return 0;
}
