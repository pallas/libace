#ifndef LACE__RANDOM_H
#define LACE__RANDOM_H

#include "try.h"
#include "pick.h"
#include "scoped.h"

#include <cstdio>
#include <cstring>

#include <unistd.h>

#ifdef __linux__
#include <sys/syscall.h>
#include <linux/random.h>
#endif

namespace lace {

class random {
public:
    random(const char * file = NULL) {
#ifdef SYS_getrandom
        if (!file)
            if (!TRY_ERR(ENOSYS, syscall, SYS_getrandom, state, sizeof state, 0))
                return;
#endif

        scoped<FILE, int, ::fclose> fd(TRY_PTR(fopen, pick(file, "/dev/urandom"), "rb"));
        TRY(fread, state, sizeof state, 1, fd);
    }

    random(unsigned short seed[3]) { memcpy(state, seed, sizeof state); }

    bool b() { return l() >= 0; }
    long l() { return jrand48(state); }
    double d() { return erand48(state); }

    unsigned long ul() { return (unsigned long)l(); }

    long max() const { return RAND_MAX; }

private:
    unsigned short state[3];
};

} // namespace lace

#endif//LACE__RANDOM_H
