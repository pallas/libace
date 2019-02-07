#ifndef LACE__OBJECTSTACK_H
#define LACE__OBJECTSTACK_H

#include "do_not_copy.h"
#include "haystack.h"

#include <cassert>
#include <type_traits>

namespace lace {

class objectstack : public lace::do_not_copy {
public:
    objectstack() : last(NULL) { }

    template <typename T, typename ...As>
    T* make(As&&... as) {
        auto w = hs.allocate<wrapper<T>>();
        if (!w)
            return NULL;

        w->that = &w->t;

        try {
            new (w->that) T(std::forward<As&&>(as)...);
        } catch(...) {
            hs.deallocate(w);
            throw;
        }

        w->link = last;
        w->destructor = !std::is_trivially_destructible<T>::value
                      ? objectstack::destroy<T> : NULL;

        last = w;
        return &w->t;
    }

    bool empty() const { return !last; }

    void unwind() {
        assert(last);
        auto old = last;
        last = old->link;

        if (old->destructor)
            old->destructor(old->that);

        hs.deallocate(old);
    }

    void clear() { while (!empty()) unwind(); }

    ~objectstack() { clear(); }

private:
    lace::haystack hs;

    struct linkage {
        linkage * link;
        void (*destructor)(void *);
        void * that;
    };
    linkage * last;

    template <typename T> struct wrapper : public linkage { T t; };
    template <typename T> static void destroy(void* t) { reinterpret_cast<T*>(t)->~T(); }
};

} // namespace lace

#endif//LACE__OBJECTSTACK_H
