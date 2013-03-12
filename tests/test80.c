#include <stdio.h>
#include "utarray.h"

int main() {
  UT_array *a;
  int i, *p;
  utarray_new(a, &ut_int_icd);
  for(i=0;i<10;i++) utarray_push_back(a,&i);
  for(p=(int*)utarray_front(a); p; p=(int*)utarray_next(a,p)) printf("%d ",*p); 
  printf("\n");
  printf("len: %u\n\n", utarray_len(a));
  
  i=11; utarray_insert(a, &i, 11);
  while ( (p=(int*)utarray_next(a,p))) printf("%d ", *p); printf("\n");
  printf("len: %u\n\n", utarray_len(a));

  utarray_free(a);
  return 0;
}

