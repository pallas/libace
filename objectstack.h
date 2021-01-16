#ifndef LACE__OBJECTSTACK_H
#define LACE__OBJECTSTACK_H

#include "do_not_copy.h"
#include "haystack.h"

#include <cassert>
#include <utility>
#include <type_traits>

namespace lace {

class objectstack : public lace::do_not_copy {
public:
    objectstack() : last(NULL) { }

    template <typename T, typename ...As>
    T* make(As&&... as) {
        auto w = hs.make<wrapper<T>>(std::forward<As&&>(as)...);
        if (!w)
            return NULL;

        w->link = last;
        w->destructor = !std::is_trivially_destructible<T>::value
                      ? &objectstack::destroy<T> : NULL;

        last = w;
        return &w->t;
    }

    bool empty() const { return !last; }

    void unwind() {
        assert(last);
        auto old = last;
        last = old->link;

        if (old->destructor)
            old->destructor(reinterpret_cast<void*>(old));

        hs.deallocate(old);
    }

    void clear() { while (!empty()) unwind(); }

    ~objectstack() { clear(); }

private:
    lace::haystack hs;

    struct linkage {
        linkage * link;
        void (*destructor)(void *);
    };
    linkage * last;

    template <typename T> struct wrapper : public linkage {
        template <typename ...As> wrapper(As&&... as) : t(std::forward<As&&>(as)...) { }
        T t;
    };
    template <typename T> static void destroy(void* w) { reinterpret_cast<wrapper<T>*>(w)->t.~T(); }
};

} // namespace lace

#endif//LACE__OBJECTSTACK_H
