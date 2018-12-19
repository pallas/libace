#include "divider.h"
#include "random.h"
#include "singleton.h"
#include <cassert>
#include <cstdint>

lace::random & rng = lace::singleton<lace::random>().instance();

int main(int, char*[]) {
  for (int i = 0 ; i < 1<<10 ; ++i) {
#ifndef NDEBUG
    lace::random o = rng;
#endif
    volatile uint64_t x = rng.l();
    assert(x == o.l());
  }
  return EXIT_SUCCESS;
}
