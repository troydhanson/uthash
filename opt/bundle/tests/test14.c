#include <stdio.h>
#include "utvector.h"

int main() {
  int i,*p=NULL;
  UT_vector v;
  utvector_init(&v, utvector_int);
  for(i=0; i<16; i++) utvector_push(&v, &i);

  p=NULL; while ( (p=(int*)utvector_next(&v,p))) printf("%d\n",*p);


  p = (int*)utvector_head(&v);
  printf("head: (%s) %d\n", p?"non-null":"null", p?*p:0);

  p = (int*)utvector_tail(&v);
  printf("tail: (%s) %d\n", p?"non-null":"null", p?*p:0);

  printf("len: %d\n", utvector_len(&v));
  printf("shift\n");
  utvector_shift(&v);
  printf("len: %d\n", utvector_len(&v));

  p = (int*)utvector_head(&v);
  printf("head: (%s) %d\n", p?"non-null":"null", p?*p:0);

  printf("clear\n");
  utvector_clear(&v);
  printf("len: %d\n", utvector_len(&v));

  utvector_fini(&v);
  return 0;
}
