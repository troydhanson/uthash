#include <stdio.h>
#include "utvector.h"

int main() {
  int i,*p=NULL;
  UT_vector v;
  utvector_init(&v, utmm_int);
  for(i=0; i<10; i++) utvector_push(&v, &i);
  while ( (p=(int*)utvector_next(&v,p))) printf("%d\n",*p);
  utvector_fini(&v);
  return 0;
}
