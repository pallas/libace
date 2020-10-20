#ifndef LACE__RANDOM_H
#define LACE__RANDOM_H

#include "try.h"
#include "pick.h"
#include "scoped.h"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>

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
    random(const seed_t seed) { reset(seed); }

    random & reset(const seed_t seed) {
        state.seed = seed;
        state.weyl = 0;
        return *this;
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

    int32_t i32() { return int32_t(u32()); }
    uint32_t u32() { return uint32_t(r()); }

    int64_t i64() { return int64_t(u64()); }
    uint64_t u64() { return (uint64_t(r())<<32) | (uint64_t(r())<<0); }

    long l() { return (long)ul(); }
    unsigned long ul() {
        return sizeof(unsigned long) <= sizeof(uint32_t) ? u32()
             : sizeof(unsigned long) <= sizeof(uint64_t) ? u64()
             : (__builtin_unreachable(), 0);
    }

    unsigned long roll(const unsigned long bound) {
        if (0 == (bound & (bound-1)))
            return ul() & (bound-1);

        const unsigned long minimum = -bound % bound;
        while (true) {
            unsigned long value = ul();
            if (value >= minimum)
                return value % bound;
        }
    }

    float f() { return float(u32()>>8)/float(UINT32_MAX>>8); }
    double d() { return double(u64()>>11)/double(UINT64_MAX>>11); }

    float g(float mean = 0.0f, float deviation = 1.0f) {
        // marsaglia-bray polar method for gaussian random number generation
        // 31 significant bits, doubled to 62, into 24 fractional bits: shift 38
        // 31 significant bits, into 24 fractional bits: shift 7

        int32_t x, y; // test coordinates
        uint32_t xxyy; // coordinate hypotenuse^2, shifted from unsigned to float

        // maximum value of int32_t, squared; shifted from unsigned to float
        static const uint32_t mm = uint64_t(int64_t(INT32_MAX) * int64_t(INT32_MAX))>>38;

        // this loop generates random coordinates in [(-1,1),(-1,1)] and
        // tests whether they fall inside a unit circle; it does this
        // without explicit conversion to floats, but does shift down to the
        // bits that will be used and later divides into ieee754
        do {
          x = i32();
          y = i32();
          xxyy = uint64_t(int64_t(x)*int64_t(x) + int64_t(y)*int64_t(y))>>38;
        } while (xxyy == 0 || xxyy > mm); // stop when inside the unit circle

        float s = float(xxyy)/float(mm); assert(s > 0.0);
        float u = float(x/(1<<7))/float(INT32_MAX/(1<<7)); // 31 significant bits, into
        //float v = float(y/(1<<7))/float(INT32_MAX/(1<<7));
        float w = M_SQRT2 * std::sqrt(-std::log(s) / s);

        return mean + deviation * u * w; // TODO: save v
    }

    double gg(double mean = 0.0, double deviation = 1.0) {
        // marsaglia-bray polar method for gaussian random number generation
        // 31 significant bits, doubled to 62, into 53 fractional bits: shift 9
        // 31 signficiant bits < 53 fractional bits: no shift

        int32_t x, y; // test coordinates
        uint64_t xxyy; // coordinate hypotenuse^2, shifted from unsigned to double

        // maximum value of int32_t, squared; shifted from unsigned to double
        static const uint64_t mm = uint64_t(int64_t(INT32_MAX) * int64_t(INT32_MAX))>>9;

        // this loop generates random coordinates in [(-1,1),(-1,1)] and
        // tests whether they fall inside a unit circle; it does this
        // without explicit conversion to doubles, but does shift down to the
        // bits that will be used and later divides into ieee754
        do {
          x = i32();
          y = i32();
          xxyy = uint64_t(int64_t(x)*int64_t(x) + int64_t(y)*int64_t(y))>>9;
        } while (xxyy == 0 || xxyy > mm); // stop when inside the unit circle

        double s = double(xxyy)/double(mm); assert(s > 0.0);
        double u = double(x)/double(INT32_MAX);
        //double v = double(y)/double(INT32_MAX);
        double w = M_SQRT2 * std::sqrt(-std::log(s) / s);

        return mean + deviation * u * w; // TODO: save v
    }

private:
    struct state_t {
        seed_t seed;
        uint64_t weyl;
    } state;

    template <uint64_t STEP = 0xb5ad4eceda1ce2a9ULL>
    uint32_t r() {
        state.seed *= state.seed;
        state.weyl += STEP | 0x1;
        state.seed += state.weyl;
        state.seed = (state.seed << 32) | (state.seed >> 32);
        return uint32_t(state.seed);
    }
};

} // namespace lace

#endif//LACE__RANDOM_H
