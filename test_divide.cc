#include "divider.h"
#include "random.h"
#include "singleton.h"
#include <cassert>
#include <cstdint>

lace::random & rng = lace::singleton<lace::random>().instance();

uint64_t r(unsigned i) {
  uint64_t m = uint64_t(1) << i%64;
  if (i<64)
      return m;
  uint64_t x = rng.l() + (uint64_t(rng.l())<<32);
  return (m-1) & x | m;
}

int main(int, char*[]) {
    for (int i = 0 ; i < 1<<10 ; ++i) {
      volatile uint64_t x = r(i);
      lace::divider d(x);
      for (int j = 0 ; j < 1<<16 ; ++j) {
          volatile uint64_t n = r(i);
          volatile uint64_t q = true ? d.modulo(n, x) : n % x;
          assert(q == n % x);
      }
    }
    return EXIT_SUCCESS;
}
