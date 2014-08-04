#include <stdio.h>
#include "utvector.h"

int main() {
  int i,*p=NULL;
  UT_vector v;
  utvector_init(&v, utvector_int);
  for(i=0; i<10; i++) utvector_push(&v, &i);
  printf("len %d\n", utvector_len(&v));
  utvector_pop(&v);
  printf("len %d\n", utvector_len(&v));
  p = (int*)utvector_extend(&v);
  *p = 11;

  p=NULL;
  while ( (p=(int*)utvector_next(&v,p))) printf("%d\n",*p);
  printf("len %d\n", utvector_len(&v));

  utvector_clear(&v);
  printf("len %d\n", utvector_len(&v));

  utvector_fini(&v);
  return 0;
}
