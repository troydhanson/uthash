#include <stdio.h>
#include "libut.h"

int main() {
  int i; UT_string *p;
  UT_vector v; utvector_init(&v, utstring_mm);
  UT_string s; utstring_init(&s);
  for(i=0; i<10; i++) {
    utstring_printf(&s, ".");
    utvector_push(&v, &s);
  }
  p=NULL;
  while ( (p=(UT_string*)utvector_next(&v,p))) printf("%s\n",utstring_body(p));

  utvector_erase(&v, 0);
  p=NULL;
  while ( (p=(UT_string*)utvector_next(&v,p))) printf("%s\n",utstring_body(p));

  utvector_erase(&v, 1);
  p=NULL;
  while ( (p=(UT_string*)utvector_next(&v,p))) printf("%s\n",utstring_body(p));


  utvector_fini(&v);
  utstring_done(&s);
  return 0;
}
