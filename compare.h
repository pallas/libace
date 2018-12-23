#ifndef LACE__COMPARE_H
#define LACE__COMPARE_H

#include <string>
#include <cwchar>
#include <cstring>
#include <type_traits>
#include <stdint.h>

namespace lace {

// return negative if foo < bar
// return zero     if foo = bar
// return positive if foo > bar

typedef int_fast8_t compare_t;

template <typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, compare_t>::type
compare(const T foo, const T bar) {
  return foo - bar;
}

template <typename T>
inline typename std::enable_if<!std::is_arithmetic<T>::value, compare_t>::type
compare(const T foo, const T bar) {
  return (bar<foo) - (foo<bar);
}

inline compare_t compare(const char * foo, const char * bar) {
  return strcmp(foo, bar);
}

inline compare_t compare(const wchar_t * foo, const wchar_t * bar) {
  return wcscmp(foo, bar);
}

inline compare_t compare(const std::string &foo, const std::string &bar) {
  return foo.compare(bar);
}

template <typename T>
compare_t reverse_compare(const T &foo, const T &bar) {
  return -compare(foo, bar);
}

template <typename T>
compare_t reverse_compare(const T foo, const T bar) {
  return -compare(foo, bar);
}

#define COMPARABLE_LT(T) \
  namespace lace { inline compare_t compare(T const &foo, T const &bar) { return (bar<foo)-(foo<bar); } }; \
  bool operator!= (T const &foo, T const &bar) { return lace::compare(foo,bar) != 0; }; \
  bool operator<= (T const &foo, T const &bar) { return lace::compare(foo,bar) <= 0; }; \
  bool operator== (T const &foo, T const &bar) { return lace::compare(foo,bar) == 0; }; \
  bool operator>= (T const &foo, T const &bar) { return lace::compare(foo,bar) >= 0; }; \
  bool operator>  (T const &foo, T const &bar) { return lace::compare(foo,bar) >  0; }; \

#define COMPARABLE(T,CMP) \
  namespace lace { inline compare_t compare(T const &foo, T const &bar) { return foo.CMP(bar); } }; \
  bool operator!= (T const &foo, T const &bar) { return lace::compare(foo,bar) != 0; }; \
  bool operator<  (T const &foo, T const &bar) { return lace::compare(foo,bar) <  0; }; \
  bool operator<= (T const &foo, T const &bar) { return lace::compare(foo,bar) <= 0; }; \
  bool operator== (T const &foo, T const &bar) { return lace::compare(foo,bar) == 0; }; \
  bool operator>= (T const &foo, T const &bar) { return lace::compare(foo,bar) >= 0; }; \
  bool operator>  (T const &foo, T const &bar) { return lace::compare(foo,bar) >  0; }; \

} // namespace lace

#endif//LACE__COMPARE_H
