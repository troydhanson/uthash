#include <stdio.h>
#include <stdlib.h>
#include "utarray.h"

typedef struct {
    int a;
    char *s;
} intchar_t;

static void intchar_copy(void *_dst, const void *_src)
{
    intchar_t *dst = (intchar_t*)_dst, *src = (intchar_t*)_src;
    dst->a = src->a;
    dst->s = (src->s != NULL) ? strdup(src->s) : NULL;
}

static void intchar_dtor(void *_elt)
{
    intchar_t *elt = (intchar_t*)_elt;
    if (elt->s != NULL) {
        free(elt->s);
    }
}

int main()
{
    UT_array *intchars;
    intchar_t ic, *p;
    UT_icd intchar_icd = {sizeof(intchar_t), NULL, intchar_copy, intchar_dtor};
    utarray_new(intchars, &intchar_icd);

    ic.a=1;
    ic.s="hello";
    utarray_push_back(intchars, &ic);
    ic.a=2;
    ic.s="world";
    utarray_push_back(intchars, &ic);

    p=NULL;
    while( (p=(intchar_t*)utarray_next(intchars,p)) != NULL ) {
        printf("%d %s\n", p->a, (p->s != NULL) ? p->s : "null");
    }

    utarray_free(intchars);
    return 0;
}

