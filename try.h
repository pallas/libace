#ifndef LACE__TRY_H
#define LACE__TRY_H

#include <cstdio>
#include <cstring>
#include <stdexcept>

#include <errno.h>

#define TRY(f, ...) ({ \
  typeof (f(__VA_ARGS__)) _r = f(__VA_ARGS__); \
  if (__builtin_expect(_r < 0, 0)) perror(#f), throw std::runtime_error(strerror(errno)); \
  _r; \
})

#define TRY_ABORT(f, ...) ({ \
  typeof (f(__VA_ARGS__)) _r = f(__VA_ARGS__); \
  if (__builtin_expect(_r < 0, 0)) perror(#f), abort(); \
  _r; \
})

#define TRY_ERR(e, f, ...) ({ \
  typeof (f(__VA_ARGS__)) _r = f(__VA_ARGS__); \
  if (__builtin_expect(_r < 0 && errno != e, 0)) perror(#f), throw std::runtime_error(strerror(errno)); \
  _r; \
})

#define TRY_PTR(f, ...) ({ \
  typeof (f(__VA_ARGS__)) _r = f(__VA_ARGS__); \
  if (__builtin_expect(!_r, 0)) perror(#f), throw std::runtime_error(strerror(errno)); \
  _r; \
})

#endif//LACE__TRY_H
