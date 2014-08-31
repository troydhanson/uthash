#include <stdio.h>
#include "libut.h"

void dump(UT_vector *v) {
  printf("len: %d\n", utvector_len(v));
  UT_string *p=NULL;
  while ( (p=(UT_string*)utvector_next(v,p))) printf("%s\n",utstring_body(p));
}

int main() {
  int i;
  UT_string *t;
  UT_vector v; utvector_init(&v, utvector_utstring);
  UT_string s; utstring_init(&s);

  for(i=0; i<16; i++) {
    utstring_printf(&s, ".");
    utvector_push(&v, &s);
  }
  dump(&v);

  printf("extend\n");
  t = (UT_string*)utvector_extend(&v);
  utstring_bincpy(t, "hello", 5);
  dump(&v);

  utvector_fini(&v);
  utstring_done(&s);
  return 0;
}
