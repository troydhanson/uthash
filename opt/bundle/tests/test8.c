#include <stdio.h>
#include "utvector.h"

int main() {
  int i,*p=NULL;
  UT_vector v;
  utvector_init(&v, utvector_int);
  for(i=0; i<16; i++) utvector_push(&v, &i);
  p = utvector_extend(&v);
  *p = 1000;
  p=NULL;
  while ( (p=(int*)utvector_next(&v,p))) printf("%d\n",*p);
  utvector_fini(&v);
  return 0;
}
