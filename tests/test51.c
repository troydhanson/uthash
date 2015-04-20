#include <stdio.h>
#include "utarray.h"

typedef struct {
    int a;
    int b;
} intpair_t;

int main()
{

    UT_array *pairs;
    intpair_t ip, *p;
    UT_icd intpair_icd = {sizeof(intpair_t), NULL, NULL, NULL};
    utarray_new(pairs,&intpair_icd);

    ip.a=1;
    ip.b=2;
    utarray_push_back(pairs, &ip);
    ip.a=10;
    ip.b=20;
    utarray_push_back(pairs, &ip);

    for(p=(intpair_t*)utarray_front(pairs);
            p!=NULL;
            p=(intpair_t*)utarray_next(pairs,p)) {
        printf("%d %d\n", p->a, p->b);
    }

    utarray_free(pairs);
    return 0;
}
