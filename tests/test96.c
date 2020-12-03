#include <stdio.h>
#include <stdlib.h>

#define HASH_FUNCTION(a,n,hv) (hv = clockface_hash(*(const int*)(a)))
#define HASH_KEYCMP(a,b,n) clockface_neq(*(const int*)(a), *(const int*)(b))

#include "uthash.h"

struct clockface {
    int time;
    UT_hash_handle hh;
};

int clockface_hash(int time)
{
    return (time % 4);
}

int clockface_neq(int t1, int t2)
{
    return ((t1 % 12) != (t2 % 12));
}

int main()
{
    int random_data[] = {
        56, 7, 10, 39, 82, 15, 31, 26, 51, 83,
        46, 92, 49, 25, 80, 54, 97, 9, 34, 86,
        87, 28, 13, 91, 95, 63, 71, 100, 44, 42,
        16, 32, 6, 85, 40, 20, 18, 99, 22, 1
    };

    struct clockface *times = NULL;
    for (int i=0; i < 40; ++i) {
        struct clockface *elt = (struct clockface *)malloc(sizeof(*elt));
        struct clockface *found = NULL;
        elt->time = random_data[i];
        HASH_FIND_INT(times, &elt->time, found);
        if (found) {
            printf("time %d found with value %d\n", elt->time, found->time);
        } else {
            printf("time %d not found, inserting it\n", elt->time);
            HASH_ADD_INT(times, time, elt);
        }
    }

    return 0;
}
