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

namespace detail {
  template <typename T, bool = std::is_arithmetic<T>::value> struct comparer;

  template <typename T>
  struct comparer<T, true> {
    inline compare_t operator() (T foo, T bar) { return foo - bar; }
  };

  template <typename T>
  struct comparer<T, false> {
    inline compare_t operator() (T foo, T bar) { return (bar<foo) - (foo<bar); }
  };

  template <>
  struct comparer<const char *> {
    inline compare_t operator() (const char * foo, const char * bar) const {
      return strcmp(foo, bar);
    }
  };
  template <> struct comparer<char *> : public comparer<const char *> { };

  template <>
  struct comparer<const wchar_t *> {
    inline compare_t operator() (const wchar_t * foo, const wchar_t * bar) const {
      return wcscmp(foo, bar);
    }
  };
  template <> struct comparer<wchar_t *> : public comparer<const wchar_t *> { };

  template <>
  struct comparer<const std::string> {
    inline compare_t operator() (const std::string &foo, const std::string &bar) const {
      return foo.compare(bar);
    }
  };
  template <> struct comparer<std::string> : public comparer<const std::string> { };

} // namespace detail

template <typename T> inline compare_t compare(const T &foo, const T &bar) { return detail::comparer<typename std::decay<T>::type>()(foo,bar); }
template <typename T> inline compare_t reverse_compare(const T &foo, const T &bar) { return -compare(foo,bar); }

#define LACE_COMPARE_OPERATOR(T,O) \
  inline bool operator O (T const &foo, T const &bar) { return lace::compare(foo,bar) O 0; };

#define LACE_COMPARABLE_LT(T) \
  namespace lace { namespace detail { \
    template <> struct comparer<const T> { \
      inline compare_t operator() (const T &foo, const T &bar) const { return (bar<foo)-(foo<bar); } \
    }; \
    template <> struct comparer<typename std::remove_const<T>::type> : public comparer<const T> { }; \
  } } \
  LACE_COMPARE_OPERATOR(T,!=); \
  LACE_COMPARE_OPERATOR(T,<=); \
  LACE_COMPARE_OPERATOR(T,==); \
  LACE_COMPARE_OPERATOR(T,>=); \
  LACE_COMPARE_OPERATOR(T,> ); \

#define LACE_COMPARABLE(T, CMP) \
  namespace lace { namespace detail { \
    template <> struct comparer<const T> { \
      inline compare_t operator() (const T &foo, const T &bar) const { return foo.CMP(bar); } \
    }; \
    template <> struct comparer<typename std::remove_const<T>::type> : public comparer<const T> { }; \
  } } \
  LACE_COMPARE_OPERATOR(T,!=); \
  LACE_COMPARE_OPERATOR(T,< ); \
  LACE_COMPARE_OPERATOR(T,<=); \
  LACE_COMPARE_OPERATOR(T,==); \
  LACE_COMPARE_OPERATOR(T,>=); \
  LACE_COMPARE_OPERATOR(T,> ); \

} // namespace lace

#endif//LACE__COMPARE_H
