#include "utvector.h"

int main() {
  UT_vector v;
  utvector_init(&v, utvector_int);
  utvector_fini(&v);
  return 0;
}
