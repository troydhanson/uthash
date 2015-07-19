#include "utvector.h"

int main() {
  UT_vector v;
  utvector_init(&v, utmm_int);
  utvector_fini(&v);
  return 0;
}
