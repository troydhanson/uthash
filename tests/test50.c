#include <stdio.h>
#include "utarray.h"

UT_icd long_icd = {sizeof(long), NULL, NULL, NULL };

int main() {
  UT_array *nums;
  long l, *p;
  utarray_new(nums, &long_icd);

  l=1; utarray_push_back(nums, &l);
  l=2; utarray_push_back(nums, &l);

  p=NULL;
  while( (p=(long*)utarray_next(nums,p))) printf("%ld\n", *p);

  utarray_free(nums);
  return 0;
}
