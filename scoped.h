#ifndef LACE__SCOPED_H
#define LACE__SCOPED_H

#include <algorithm>

#include "do_not_copy.h"

namespace lace {

template <typename T, typename R, R (*D)(T*)>
class scoped : public lace::do_not_copy {
public:
    scoped(T* _) : p(_) { }
    ~scoped() { if (p) D(p); }

    operator T* () const { return p; }
    T* operator->() const { return p; }

    T* reset(T* r = NULL) {
        std::swap(r, p);
        return r;
    }

    void clear(T* r = NULL) {
        if (p) D(p);
        p = r;
    }

private:
    T* p;
};

} // namespace lace

#endif//LACE__SCOPED_H
