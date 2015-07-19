#include <stdio.h>
#include "libut.h"

void dump(UT_vector *v) {
  printf("len: %d\n", utvector_len(v));
  UT_string *p=NULL;
  while ( (p=(UT_string*)utvector_next(v,p))) printf("%s\n",utstring_body(p));
}

int main() {
  int i; UT_string *p;
  UT_vector v; utvector_init(&v, utstring_mm);
  UT_string s; utstring_init(&s);
  for(i=0; i<10; i++) {
    utstring_printf(&s, ".");
    utvector_push(&v, &s);
  }
  dump(&v);

  printf("extend\n");
  p = utvector_extend(&v);
  utstring_printf(p,"extended element");
  dump(&v);

  printf("pop\n");
  utvector_pop(&v);
  dump(&v);

  printf("clear\n");
  utvector_clear(&v);
  dump(&v);

  printf("push\n");
  utstring_new(p);
  utstring_printf(p,"allocated utstring");
  utvector_push(&v, p);
  utstring_free(p);
  dump(&v);

  printf("done1\n");
  utvector_fini(&v);
  printf("done2\n");
  utstring_done(&s);
  return 0;
}
