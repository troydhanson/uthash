#include <stdio.h>
#include "utarray.h"

int main(void) {
    UT_array *nums;
    int i, *p;

    utarray_new(nums,&ut_int_icd);
    for(i=0; i < 10; i++) utarray_push_back(nums,&i);

    i = 110;
    utarray_replace(nums, 5, &i);

    for(p=(int*)utarray_front(nums);
        p!=NULL;
        p=(int*)utarray_next(nums,p)) {
        printf("%d\n",*p);
    }


    utarray_free(nums);

    return 0;
}
