#ifndef LACE__RANDOM_H
#define LACE__RANDOM_H

#include "try.h"
#include "pick.h"
#include "scoped.h"

#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <stdint.h>

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
            if (!TRY_ERR(ENOSYS, syscall, SYS_getrandom, &state, sizeof state, 0))
                return;
#endif

        scoped<FILE, int, ::fclose> fd(TRY_PTR(fopen, pick(file, "/dev/urandom"), "rb"));
        TRY(fread, &state, sizeof state, 1, fd);
    }

    typedef uint64_t seed_t;
    random(const seed_t seed) {
        memcpy(&state.seed, &seed, sizeof state.seed);
    }

    bool b() {
        uint32_t c = u32();
        c = (c & 0x55555555UL) + ((c>> 1) & 0x55555555UL);
        c = (c & 0x33333333UL) + ((c>> 2) & 0x33333333UL);
        c = (c & 0x0f0f0f0fUL) + ((c>> 4) & 0x0f0f0f0fUL);
        c = (c & 0x00ff00ffUL) + ((c>> 8) & 0x00ff00ffUL);
        c = (c & 0x0000ffffUL) + ((c>>16) & 0x0000ffffUL);
        return c & 0x1;
    }

    int32_t i32() { return (int32_t)u32(); }
    uint32_t u32() { return (uint32_t)r(); }

    int64_t i64() { return (int64_t)u64(); }
    uint64_t u64() { return (r()<<32) | (r()>>32); }

    long l() { return (long)ul(); }
    unsigned long ul() {
        return sizeof(unsigned long) <= sizeof(uint32_t) ? u32()
             : sizeof(unsigned long) <= sizeof(uint64_t) ? u64()
             : (__builtin_unreachable(), 0);
    }

    double d() { return double(u64()>>11)/double(UINT64_MAX>>11); }

private:
    struct state_t {
        seed_t seed;
        uint64_t weyl;
    } state;

    template <uint64_t STEP = 0xb5ad4eceda1ce2a9ULL>
    uint64_t r() {
        state.seed *= state.seed;
        state.weyl += STEP;
        state.seed += state.weyl;
        state.seed = (state.seed << 32) | (state.seed >> 32);
        return state.seed;
    }
};

} // namespace lace

#endif//LACE__RANDOM_H
