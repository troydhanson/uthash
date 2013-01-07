#include <stdio.h>
#include "utarray.h"

int main() {
  UT_array *a;
  int i, *p=NULL;
  utarray_new(a, &ut_int_icd);
  for(i=0;i<10;i++) utarray_push_back(a,&i);
  utarray_pop_back(a);
  utarray_erase(a,0,1);
  while ( (p=(int*)utarray_next(a,p))) printf("%d ",*p); printf("\n");
  i = 100;
  utarray_insert(a,&i,3);
  while ( (p=(int*)utarray_next(a,p))) printf("%d ",*p); printf("\n");
  utarray_extend_back(a);
  p = (int*)utarray_back(a);
  *p = 1000;
  p = NULL;
  while ( (p=(int*)utarray_next(a,p))) printf("%d ",*p); printf("\n");
  utarray_clear(a);
  utarray_free(a);
  return 0;
}

