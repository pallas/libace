#ifndef LACE__HASH_H
#define LACE__HASH_H

#include <string>
#include <cwchar>
#include <cstring>
#include <stdint.h>

namespace lace {

typedef uint64_t hash_t;
typedef uint_fast64_t fast_hash_t;

namespace detail{
  inline hash_t
  fnv_1a(const unsigned char * d, size_t l) {
    fast_hash_t h = 0xcbf29ce484222325ULL;
    while (l--) {
      h ^= *d++;
      h *= 0x100000001b3ULL;
    }
    return h;
  }
} // namespace detail

template <typename T> hash_t hash(T const &t) {
  return detail::fnv_1a(reinterpret_cast<const unsigned char *>(&t), sizeof(t));
}

template <> inline hash_t hash(const char * const &t) {
  return detail::fnv_1a(reinterpret_cast<const unsigned char *>(t), strlen(t));
}

template <> inline hash_t hash(const wchar_t * const &t) {
  return detail::fnv_1a(reinterpret_cast<const unsigned char *>(t), wcslen(t) * sizeof(wchar_t));
}

template <> inline hash_t hash(std::string const &s) {
  return detail::fnv_1a(reinterpret_cast<const unsigned char *>(s.data()), s.length());
}

} // namespace lace

#endif//LACE__HASH_H
