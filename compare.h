#ifndef LACE__COMPARE_H
#define LACE__COMPARE_H

#include <string>
#include <cwchar>
#include <cstring>
#include <stdint.h>

namespace lace {

// return -1 if foo < bar
// return  0 if foo = bar
// return +1 if foo > bar

typedef int_fast8_t compare_t;

template <typename T>
compare_t compare(T const &foo, T const &bar) {
  return (bar<foo) - (foo<bar);
}

template <>
inline compare_t compare(const char * const &foo, const char * const &bar) {
  return compare<int>(strcmp(foo, bar), 0);
}

template <>
inline compare_t compare(const wchar_t * const &foo, const wchar_t * const &bar) {
  return compare<int>(wcscmp(foo, bar), 0);
}

template <>
inline compare_t compare(std::string const &foo, std::string const &bar) {
  return compare<int>(foo.compare(bar), 0);
}

template <typename T>
compare_t reverse_compare(T const &foo, T const &bar) {
  return -compare(foo, bar);
}

} // namespace lace

#endif//LACE__COMPARE_H
