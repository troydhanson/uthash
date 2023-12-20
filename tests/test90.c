#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

struct item {
    void *sort_field;
    size_t sort_field_len; /** Sort field length, in bytes */
    int some_user_data;
    UT_hash_handle hh;
};

int sort_func(const struct item *a, const struct item *b)
{
    int va = *(int*)a->sort_field;
    int vb = *(int*)b->sort_field;
    return (va < vb) ? -1 : (va > vb);
}

int main()
{
    int i;
    struct item *p, *tmp;
    int total = 0;

    /** The sorted list */
    struct item *list = NULL;

    /* fill in the sorted list */
    for (i = 0; i < 100; i += 2) {
        p = (struct item *)malloc(sizeof *p);

        p->sort_field_len = sizeof(int);
        p->sort_field = malloc(p->sort_field_len);
        *(int*)p->sort_field = i;

        HASH_ADD_KEYPTR_INORDER(hh, list, p->sort_field, p->sort_field_len, p, sort_func);
    }

    printf("filling in is ok\n");

    for (i = 0; i < 100; ++i) {
        p = (struct item *)malloc(sizeof *p);

        p->sort_field_len = sizeof(int);
        p->sort_field = malloc(p->sort_field_len);
        *(int*)p->sort_field = i;

        HASH_REPLACE_KEYPTR_INORDER(hh, list, p->sort_field, p->sort_field_len, p, tmp, sort_func);
        if (tmp != NULL) {
            free(tmp->sort_field);
            free(tmp);
        }
    }

    printf("replacing is ok\n");

    i = 0;
    HASH_ITER(hh, list, p, tmp) {
        int value = *(int*)(void*)p->sort_field;
        assert(i == value);
        total += value;
        HASH_DEL(list, p);
        free(p->sort_field);
        free(p);
        i += 1;
    }
    printf("%d\n", total);

    return 0;
}
