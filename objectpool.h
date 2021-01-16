#ifndef LACE__OBJECTPOOL_H
#define LACE__OBJECTPOOL_H

#include "container_of.h"
#include "do_not_copy.h"
#include "haystack.h"

#include <cassert>
#include <utility>
#include <type_traits>

namespace lace {

template <typename TYPE, bool autodestruct = !std::is_trivially_destructible<TYPE>::value>
class objectpool : public lace::do_not_copy {
public:
    typedef TYPE type_t;

    objectpool() {
        lists[FREELIST] = sentinel();
        if (autodestruct)
            lists[FULLLIST] = NULL;
        hs.align<wrapper>();
    }

    template <typename ...As>
    type_t* allocate(As&&... as) {
        auto w = getfree();
        if (!w)
            return NULL;

        try {
            new (&w->object) type_t(std::forward<As&&>(as)...);
        } catch(...) {
            putfree(w);
            throw;
        }

        return &w->object;
    }

    void deallocate(type_t* t) {
        t->~TYPE();
        putfree(&lace::container_of(*t, &wrapper::object));
    }

    void clear() {
        if (autodestruct) {
            while (lists[FULLLIST]) {
                if (!freed(lists[FULLLIST]))
                    lists[FULLLIST]->object.~TYPE();
                lists[FULLLIST] = lists[FULLLIST]->links[FULLLIST];
            }
            assert(!lists[FULLLIST]);
        }

        lists[FREELIST] = NULL;
        hs.clear();
    }

    ~objectpool() { clear(); }

private:
    lace::haystack hs;

    struct wrapper_struct {
        wrapper_struct * links[2];
        type_t object;
    };

    union wrapper_union {
        wrapper_union * links[1];
        type_t object;
    };

    typedef typename std::conditional<autodestruct, wrapper_struct, wrapper_union>::type wrapper;

    wrapper * lists[autodestruct ? 2 : 1];

    enum {
        FREELIST,
        FULLLIST,
    };

    bool freed(wrapper* w) const {
        assert(autodestruct);
        return w->links[FREELIST]; // sentinel -> true
    }

    void putfree(wrapper* w) {
        w->links[FREELIST] = lists[FREELIST];
        lists[FREELIST] = w;
    }

    wrapper* getfree() {
        if (lists[FREELIST] != sentinel()) {
            auto w = lists[FREELIST];
            lists[FREELIST] = w->links[FREELIST];
            w->links[FREELIST] = NULL;
            return w;
        } else if (auto w = hs.allocate<wrapper>()) {
            if (autodestruct) {
                w->links[FULLLIST] = lists[FULLLIST];
                lists[FULLLIST] = w;
            }
            w->links[FREELIST] = NULL;
            return w;
        } else {
            return NULL;
        }
    }

    wrapper* sentinel() const { return const_cast<wrapper*>(reinterpret_cast<const wrapper*>(this)); }
};

} // namespace lace

#endif//LACE__OBJECTPOOL_H
