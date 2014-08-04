#include <stdio.h>
#include "utvector.h"

int main() {
  int i,*p=NULL;
  UT_vector v,*k;
  utvector_init(&v, utvector_int);
  for(i=0; i<10; i++) utvector_push(&v, &i);

  k = utvector_clone(&v);
  while ( (p=(int*)utvector_next(k,p))) printf("%d\n",*p);

  utvector_fini(&v);
  utvector_free(k);
  return 0;
}
