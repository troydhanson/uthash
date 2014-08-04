#include <stdio.h>
#include "libut.h"

int main() {
  UT_string *s;
  UT_vector v;
  utvector_init(&v, utvector_utstring);

  printf("extend\n");
  s = (UT_string*)utvector_extend(&v);
  utstring_printf(s,"hello");

  printf("iterate\n");
  s=NULL;
  while ( (s=(UT_string*)utvector_next(&v,s))) printf("%s\n",utstring_body(s));

  printf("pop\n");
  s=(UT_string*)utvector_pop(&v);
  printf("%s\n", s ? utstring_body(s) : "(null)");

  printf("iterate\n");
  s=NULL;
  while ( (s=(UT_string*)utvector_next(&v,s))) printf("%s\n",utstring_body(s));

  printf("pop\n");
  s=(UT_string*)utvector_pop(&v);
  printf("%s\n", s ? utstring_body(s) : "(null)");

  printf("iterate\n");
  s=NULL;
  while ( (s=(UT_string*)utvector_next(&v,s))) printf("%s\n",utstring_body(s));

  utvector_fini(&v);
  return 0;
}
