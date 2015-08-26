#include <stdio.h>
#include "utvector.h"

int main() {
  int *p;
  UT_vector v;
  utvector_init(&v, utmm_int);

  printf("extend\n");
  p = (int*)utvector_extend(&v);
  *p = 1000;
  p=NULL;
  while ( (p=(int*)utvector_next(&v,p))) printf("%d\n",*p);

  printf("pop\n");
  p = (int*)utvector_pop(&v);
  printf("result: %s %d\n", p ? "non-null" : "null", p ? *p : 0);

  printf("pop\n");
  p = (int*)utvector_pop(&v);
  printf("result: %s %d\n", p ? "non-null" : "null", p ? *p : 0);

  utvector_fini(&v);
  return 0;
}
