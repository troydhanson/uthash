#include <assert.h>
#include <stddef.h>
#include "uthash.h"

typedef struct {
    void *key;
    int i;
    UT_hash_handle hh;
} el_t;

el_t *findit(el_t *hash, void *keytofind)
{
    el_t *found;
    HASH_FIND_PTR(hash, &keytofind, found);
    return found;
}

int main()
{
    el_t *hash = NULL;
    el_t e1;
    el_t e2;

    e1.key = NULL;
    e1.i = 1;

    e2.key = &e2;
    e2.i = 2;

    assert(findit(hash, NULL) == NULL);
    assert(findit(hash, &e1) == NULL);
    assert(findit(hash, &e2) == NULL);

    HASH_ADD_PTR(hash, key, &e1);
    assert(findit(hash, NULL) == &e1);
    assert(findit(hash, &e1) == NULL);
    assert(findit(hash, &e2) == NULL);

    HASH_ADD_PTR(hash, key, &e2);
    assert(findit(hash, NULL) == &e1);
    assert(findit(hash, &e1) == NULL);
    assert(findit(hash, &e2) == &e2);

    HASH_DEL(hash, &e1);
    assert(findit(hash, NULL) == NULL);
    assert(findit(hash, &e1) == NULL);
    assert(findit(hash, &e2) == &e2);

    HASH_CLEAR(hh, hash);
    assert(hash == NULL);

    return 0;
}
