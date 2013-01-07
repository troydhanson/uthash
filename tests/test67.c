#include <stdio.h>
#include "utarray.h"

int main() {
  UT_array *nums;
  int i, *p;

  utarray_new(nums,&ut_int_icd);
  for(i=0; i < 10; i++) utarray_push_back(nums,&i);

  for(p=(int*)utarray_back(nums);
      p!=NULL;
      p=(int*)utarray_prev(nums,p)) {
    printf("%d\n",*p);
  }

  /* the other form of iteration starting from NULL (back) */
  p=NULL;
  while ( (p=(int*)utarray_prev(nums,p))) {
    printf("%d\n",*p);
  }


  utarray_free(nums);

  return 0;
}
