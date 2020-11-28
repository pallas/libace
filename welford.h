#ifndef LACE__WELFORD_H
#define LACE__WELFORD_H

#include <cmath>

namespace lace {

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

} // namespace lace

#endif//LACE__WELFORD_H
