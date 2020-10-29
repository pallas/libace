#include "random.h"
#include "singleton.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <iostream>

lace::random & rng = lace::singleton<lace::random>::instance();

template <typename T = double>
class welford {
public:
  welford() : _count(0), _mean(T(0.0)), _squared_distance(T(0.0)) { }

  welford & operator<< (T value) {
    T delta = value - _mean;
    _mean += delta / ++_count;
    _squared_distance += delta * (value - _mean);
    return *this;
  }

  T samples() const { return _count; }
  T mean() const { return _mean; }
  T variance() const { return _squared_distance / T(_count - 1); }
  T deviation() const { return std::sqrt(variance()); }
  T error() const { return std::sqrt(variance()/samples()); }
  T gosset(T u) { return (_mean - u) * std::sqrt(samples()/variance()); }

private:
  unsigned _count;
  T _mean, _squared_distance;
};

int main(int, char*[]) {
  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << i % 2;

    std::cout << "perfect coin"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(0.5)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.b();

    std::cout << "pop"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(0.5)
      << std::endl;
  }}

  {{
    const unsigned bits = sizeof(long)*CHAR_BIT;
    welford<> w[bits];
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i) {
      unsigned long x = rng.ul();
      for (int j = 0 ; j < bits ; ++j)
        w[j] << ((x & (0x1ULL<<j))>>j);
    }

    welford<> wx;
    for (int j = 0 ; j < bits ; ++j) {
      wx << w[j].mean();
      std::cout << "bit " << j
        << ",\t mean=" << w[j].mean()
        << ",\t variance=" << w[j].variance()
        << ",\t error=" << w[j].error()
        << ",\t t-test=" << w[j].gosset(0.5)
        << std::endl;
    }

    std::cout << "bits"
      << ",\t mean=" << wx.mean()
      << ",\t variance=" << wx.variance()
      << ",\t error=" << wx.error()
      << ",\t t-test=" << wx.gosset(0.5)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.f();

    std::cout << "f"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(0.5)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.d();

    std::cout << "d"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(0.5)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.g(0,1);

    std::cout << "g(0,1)"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t deviation=" << w.deviation()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(0)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.g(1,3);

    std::cout << "g(1,3)"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t deviation=" << w.deviation()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(1)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.g(2,7);

    std::cout << "g(2,7)"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t deviation=" << w.deviation()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(2)
      << std::endl;
  }}

  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.gg(4,15);

    std::cout << "gg(4,15)"
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t deviation=" << w.deviation()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(4)
      << std::endl;
  }}

  for (int d = 1 ; d <= 20 ; ++d)
  {{
    welford<> w;
    const int n = 1<<20;
    for (int i = 0 ; i < n ; ++i)
      w << rng.roll(d) + 1;
    std::cout << "d" << d
      << ",\t mean=" << w.mean()
      << ",\t variance=" << w.variance()
      << ",\t deviation=" << w.deviation()
      << ",\t error=" << w.error()
      << ",\t t-test=" << w.gosset(float(d+1)/2.0f)
      << std::endl;
  }}

  return EXIT_SUCCESS;
}
