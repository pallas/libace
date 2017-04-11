#ifndef LACE__DIVIDER_H
#define LACE__DIVIDER_H

#include <cassert>
#include <climits>
#include <cstdlib>
#include <stdint.h>

namespace lace {

class divider {
public:
    typedef uint8_t bits_t;
    typedef uint64_t type_t;
    static const bits_t type_bits = CHAR_BIT * sizeof(type_t);
    typedef __uint128_t type_type_t;

    divider(type_t d = 1) { invert(d); }

    void invert(type_t d) {
        if (0 == d) {
            method = &divider::zero;
            return;
        }

        if (1 == d) {
            method = &divider::identity;
            return;
        }

        bits_t b = logbits(d);

        if (power_of_two(d)) {
            method = shifter_method(b);
            return;
        }

        type_type_t x = type_type_t(1) << (b+type_bits);
        type_t q = x / d;
        type_t r = x % d;

        if (d - r < (type_t(1)<<b)) {
            method = simple_method(b);
            magic_number = q + 1;
            return;
        }

        method = extended_method(b);
        magic_number = 1 + (q<<1);

        type_t rr = r<<1;
        if (rr < r || rr >= d)
            magic_number += 1;
    }

    type_t divide(type_t n) const {
        assert(method);
        return (*method)(n, magic_number);
    }

    type_t modulo(type_t n, type_t d) const {
        type_t r = n - divide(n) * d;
        assert(r == n % d);
        return r;
    }

private:
    static bool power_of_two(const type_t t) { return 0 == (t & (t-1)); }

    static bits_t logbits(const type_t t) {
        assert(t > 0);
        assert(sizeof(unsigned long long) >= sizeof(type_t));
        static const bits_t b = CHAR_BIT * sizeof(type_t);
        return b - 1 - __builtin_clzll(t);
    }

    typedef type_t (*method_t)(type_t, type_t);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
    type_t static zero(type_t n, type_t) { return n/0; }
#pragma GCC diagnostic pop

    type_t static identity(type_t n, type_t) { return n; }

    template <bits_t S> type_t static shifter(type_t n, type_t) {
        assert(S > 0);
        assert(S < type_bits);
        return n>>S;
    };

    static method_t shifter_method(bits_t b) {
        switch(b) {
#define X(x) case x: return &divider::shifter<x>
                 X(0x01); X(0x02); X(0x03); X(0x04); X(0x05); X(0x06); X(0x07);
        X(0x08); X(0x09); X(0x0a); X(0x0b); X(0x0c); X(0x0d); X(0x0e); X(0x0f);
        X(0x10); X(0x11); X(0x12); X(0x13); X(0x14); X(0x15); X(0x16); X(0x17);
        X(0x18); X(0x19); X(0x1a); X(0x1b); X(0x1c); X(0x1d); X(0x1e); X(0x1f);
        X(0x20); X(0x21); X(0x22); X(0x23); X(0x24); X(0x25); X(0x26); X(0x27);
        X(0x28); X(0x29); X(0x2a); X(0x2b); X(0x2c); X(0x2d); X(0x2e); X(0x2f);
        X(0x30); X(0x31); X(0x32); X(0x33); X(0x34); X(0x35); X(0x36); X(0x37);
        X(0x38); X(0x39); X(0x3a); X(0x3b); X(0x3c); X(0x3d); X(0x3e); X(0x3f);
#undef X
        default:
            assert(!"invalid shift");
            return NULL;
        }
    }

    template <bits_t S> static type_t simple(type_t n, type_t m) {
        assert(S > 0);
        assert(S < type_bits);
        return (type_type_t(n)*m) >> (type_bits + S);
    };

    static method_t simple_method(bits_t b) {
        switch(b) {
#define X(x) case x: return &divider::simple<x>
                 X(0x01); X(0x02); X(0x03); X(0x04); X(0x05); X(0x06); X(0x07);
        X(0x08); X(0x09); X(0x0a); X(0x0b); X(0x0c); X(0x0d); X(0x0e); X(0x0f);
        X(0x10); X(0x11); X(0x12); X(0x13); X(0x14); X(0x15); X(0x16); X(0x17);
        X(0x18); X(0x19); X(0x1a); X(0x1b); X(0x1c); X(0x1d); X(0x1e); X(0x1f);
        X(0x20); X(0x21); X(0x22); X(0x23); X(0x24); X(0x25); X(0x26); X(0x27);
        X(0x28); X(0x29); X(0x2a); X(0x2b); X(0x2c); X(0x2d); X(0x2e); X(0x2f);
        X(0x30); X(0x31); X(0x32); X(0x33); X(0x34); X(0x35); X(0x36); X(0x37);
        X(0x38); X(0x39); X(0x3a); X(0x3b); X(0x3c); X(0x3d); X(0x3e); X(0x3f);
#undef X
        default:
            assert(!"invalid shift");
            return NULL;
        }
    }

    template <bits_t S> static type_t extended(type_t n, type_t m) {
        assert(S > 0);
        assert(S < type_bits);
        type_t x = (type_type_t(n)*m) >> type_bits;
        return (x + ((n-x)>>1)) >> S;
    };

    static method_t extended_method(bits_t b) {
        switch(b) {
#define X(x) case x: return &divider::extended<x>
                 X(0x01); X(0x02); X(0x03); X(0x04); X(0x05); X(0x06); X(0x07);
        X(0x08); X(0x09); X(0x0a); X(0x0b); X(0x0c); X(0x0d); X(0x0e); X(0x0f);
        X(0x10); X(0x11); X(0x12); X(0x13); X(0x14); X(0x15); X(0x16); X(0x17);
        X(0x18); X(0x19); X(0x1a); X(0x1b); X(0x1c); X(0x1d); X(0x1e); X(0x1f);
        X(0x20); X(0x21); X(0x22); X(0x23); X(0x24); X(0x25); X(0x26); X(0x27);
        X(0x28); X(0x29); X(0x2a); X(0x2b); X(0x2c); X(0x2d); X(0x2e); X(0x2f);
        X(0x30); X(0x31); X(0x32); X(0x33); X(0x34); X(0x35); X(0x36); X(0x37);
        X(0x38); X(0x39); X(0x3a); X(0x3b); X(0x3c); X(0x3d); X(0x3e); X(0x3f);
#undef X
        default:
            assert(!"invalid shift");
            return NULL;
        }
    }

    method_t method;
    type_t magic_number;
};

} // namespace lace

#endif//LACE__DIVIDER_H
