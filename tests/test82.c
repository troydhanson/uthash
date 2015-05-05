#include <stdio.h>
#include "utarray.h"

int main()
{
    UT_array *a,*b;
    int i, *p;
    utarray_new(a, &ut_int_icd);
    utarray_new(b, &ut_int_icd);

    for(i=0; i<10; i++) {
        utarray_push_back(a,&i);
    }
    for(p=(int*)utarray_front(a); p!=NULL; p=(int*)utarray_next(a,p)) {
        printf("%d ",*p);
    }
    printf("\n");
    printf("len: %u\n\n", utarray_len(a));

    utarray_inserta(b,a,10);
    for(p=(int*)utarray_front(b); p!=NULL; p=(int*)utarray_next(b,p)) {
        printf("%d ",*p);
    }
    printf("len: %u\n\n", utarray_len(b));

    utarray_inserta(b,a,0);
    for(p=(int*)utarray_front(b); p!=NULL; p=(int*)utarray_next(b,p)) {
        printf("%d ",*p);
    }
    printf("len: %u\n\n", utarray_len(b));

    utarray_inserta(b,a,25);
    for(p=(int*)utarray_front(b); p!=NULL; p=(int*)utarray_next(b,p)) {
        printf("%d ",*p);
    }
    printf("len: %u\n\n", utarray_len(b));

    utarray_free(a);
    utarray_free(b);
    return 0;
}

