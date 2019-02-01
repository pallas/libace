#include "divider.h"
#include "random.h"
#include "singleton.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>

lace::random & rng = lace::singleton<lace::random>::instance();

lace::divider::type_t r(unsigned i) {
  lace::divider::type_t m = lace::divider::type_t(1) << i%lace::divider::type_bits;
  if (i<lace::divider::type_bits)
      return m;
  lace::divider::type_t x = rng.l() + (lace::divider::type_t(rng.l())<<(lace::divider::type_bits/2));
  return ((m-1) & x) | m;
}

int main(int, char*[]) {
    for (int i = 0 ; i < 1<<10 ; ++i) {
      volatile lace::divider::type_t x = r(i);
      lace::divider d(x);
      for (int j = 0 ; j < 1<<16 ; ++j) {
          volatile lace::divider::type_t n = r(i);
          volatile lace::divider::type_t q = true ? d.modulo(n, x) : n % x;
          assert(q == n % x);
      }
    }
    return EXIT_SUCCESS;
}
