#include <stdio.h>
#include "utarray.h"

int main(void) {
    UT_array *nums;
    int i, *p;

    utarray_new(nums, &ut_int_icd);

    for(i = 1; i < 1024; i <<= 1){
      utarray_push_back(nums, &i);
    }

    i = 110;
    utarray_replace(nums, &i, 5);

    for(p = (int*)utarray_front(nums);
        p != NULL;
        p = (int*)utarray_next(nums, p)) {
        printf("%d\n",*p);
    }

    utarray_free(nums);
    return 0;
}
