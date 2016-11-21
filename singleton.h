#ifndef LACE__SINGLETON_H
#define LACE__SINGLETON_H

#include "do_not_copy.h"

namespace lace {

template <class T>
class singleton : public do_not_copy {
public:
  static T & instance() {
    instantiate();
    static T t;
    return t;
  }

private:
  static struct instantiator {
    instantiator() { singleton::instance(); }
    void operator()() { };
  } instantiate;
};

template <class T> struct singleton<T>::instantiator singleton<T>::instantiate;

} // namespace lace

#endif//LACE__SINGLETON_H
