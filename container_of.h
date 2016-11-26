#ifndef LACE__CONTAINER_OF_H
#define LACE__CONTAINER_OF_H

#include <cstddef>

namespace lace {

template <class T, typename M>
T & container_of(M & m, const M T::*mp) {
  static const T * null = reinterpret_cast<const T *>(NULL);
  const ptrdiff_t offset = reinterpret_cast<const ptrdiff_t>(&(null->*mp));
  return *reinterpret_cast<T*>(reinterpret_cast<char*>(&m) - offset);
}

} // namespace lace

#endif//LACE__CONTAINER_OF_H
