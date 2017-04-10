#include "divider.h"
#include <cassert>
#include <cstdlib>
#include <unistd.h>

uint64_t r(unsigned i) {
  return i<32 ? uint64_t(1)<<i
    : i%2 ? rand() : rand() + uint64_t(rand())<<32;
}

int main(int, char*[]) {
    srand(getpid());
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
