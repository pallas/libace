#ifndef LACE__HASH_H
#define LACE__HASH_H

#include <string>
#include <cwchar>
#include <cstring>
#include <type_traits>
#include <stdint.h>

namespace lace {

typedef uint64_t hash_t;
typedef uint_fast64_t fast_hash_t;

namespace detail {
  inline hash_t
  fnv_1a(const unsigned char * d, size_t l) {
    fast_hash_t h = 0xcbf29ce484222325ULL;
    while (l--) {
      h ^= *d++;
      h *= 0x100000001b3ULL;
    }
    return h;
  }

  template <typename T>
  struct hasher {
    inline hash_t operator() (T t) const {
      return detail::fnv_1a(reinterpret_cast<const unsigned char *>(&t), sizeof(t));
    }
  };

  template <>
  struct hasher<const char *> {
    inline hash_t operator() (const char * s) const {
      return detail::fnv_1a(reinterpret_cast<const unsigned char *>(s), strlen(s));
    }
  };
  template <> struct hasher<char *> : public hasher<const char *> { };

  template <>
  struct hasher<const wchar_t *> {
    inline hash_t operator() (const wchar_t * s) const {
      return detail::fnv_1a(reinterpret_cast<const unsigned char *>(s), wcslen(s) * sizeof(wchar_t));
    }
  };
  template <> struct hasher<wchar_t *> : public hasher<const wchar_t *> { };

  template <>
  struct hasher<const std::string> {
    inline hash_t operator() (const std::string &s) const {
      return detail::fnv_1a(reinterpret_cast<const unsigned char *>(s.data()), s.length());
    }
  };
  template <> struct hasher<std::string> : public hasher<const std::string> { };

} // namespace detail

template <typename T> inline hash_t hash(const T &t) { return detail::hasher<typename std::decay<T>::type>()(t); }

} // namespace lace

#define LACE_HASHABLE(T, HC) \
namespace lace { namespace detail { \
  template <> struct hasher<const T> { \
    inline hash_t operator() (const T &s) const { return s.HC(); } \
  }; \
  template <> struct hasher<typename std::remove_const<T>::type> : public hasher<const T> { }; \
} }

#endif//LACE__HASH_H
