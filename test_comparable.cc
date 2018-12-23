#include "compare.h"
#include <cassert>
#include <cstdlib>

struct nodeX {
  nodeX(int xx) : x(xx) { }
  lace::compare_t cmp(const nodeX &that) const { return lace::compare(x, that.x); }
private:
  int x;
};
COMPARABLE(nodeX, cmp);

struct nodeLT {
  nodeLT(int xx) : x(xx) { }
  friend bool operator<(const nodeLT &foo, const nodeLT &bar) { return foo.x < bar.x; }
private:
  int x;
};
COMPARABLE_LT(nodeLT);

template <typename T> void test() {
  assert( (T(-1) != T(+1)) );
  assert( (T(-1) <  T(+1)) );
  assert( (T(-1) <= T(+1)) );
  assert(!(T(-1) == T(+1)) );
  assert(!(T(-1) >= T(+1)) );
  assert(!(T(-1) >  T(+1)) );

  assert(!(T( 0) != T( 0)) );
  assert(!(T( 0) <  T( 0)) );
  assert( (T( 0) <= T( 0)) );
  assert( (T( 0) == T( 0)) );
  assert( (T( 0) >= T( 0)) );
  assert(!(T( 0) >  T( 0)) );

  assert( (T(+1) != T(-1)) );
  assert(!(T(+1) <  T(-1)) );
  assert(!(T(+1) <= T(-1)) );
  assert(!(T(+1) == T(-1)) );
  assert( (T(+1) >= T(-1)) );
  assert( (T(+1) >  T(-1)) );
}

int main(int, char*[]) {
  test<int>();
  test<nodeX>();
  test<nodeLT>();

  assert( (lace::compare("a","b") != 0));
  assert( (lace::compare("a","b") <  0));
  assert( (lace::compare("a","b") <= 0));
  assert(!(lace::compare("a","b") == 0));
  assert(!(lace::compare("a","b") >= 0));
  assert(!(lace::compare("a","b") >  0));

  assert(!(lace::compare("a","a") != 0));
  assert(!(lace::compare("a","a") <  0));
  assert( (lace::compare("a","a") <= 0));
  assert( (lace::compare("a","a") == 0));
  assert( (lace::compare("a","a") >= 0));
  assert(!(lace::compare("a","a") >  0));

  assert(!(lace::compare(L"a", L"a") != 0));
  assert(!(lace::compare(std::string("a"),std::string("a")) != 0));

  return EXIT_SUCCESS;
}
