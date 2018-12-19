#ifndef LACE__PICK_H
#define LACE__PICK_H

#include <cstdarg>

namespace lace {

template <typename TYPE>
TYPE pick(TYPE t0, TYPE tx,...) {
    if (t0)
        return t0;

    if (!tx) {
        va_list args;
        va_start(args, tx);
        while (!tx)
            tx = va_arg(args, TYPE);
        va_end(args);
    }

    return tx;
}

} // namespace lace

#endif//LACE__PICK_H
