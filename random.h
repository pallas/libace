#ifndef LACE__RANDOM_H
#define LACE__RANDOM_H

#include "try.h"
#include "scoped.h"

#include <cstdio>
#include <cstring>

namespace lace {

class random {
public:
    random(const char * file = "/dev/urandom") {
        scoped<FILE, int, ::fclose> fd(TRY_PTR(fopen, file, "rb"));
        TRY(fread, state, sizeof state, 1, fd);
    }

    random(unsigned short seed[3]) { memcpy(state, seed, sizeof state); }

    long l() { return jrand48(state); }
    double d() { return erand48(state); }

    unsigned long ul() { return (unsigned long)l(); }

    long max() const { return RAND_MAX; }

private:
    unsigned short state[3];
};

} // namespace lace

#endif//LACE__RANDOM_H
