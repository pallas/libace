#ifndef LACE__HAYSTACK_H
#define LACE__HAYSTACK_H

#include "do_not_copy.h"
#include "try.h"

#include <cstdlib>
#include <cstdarg>
#include <cassert>

#include <obstack.h>

namespace lace {

class haystack : public lace::do_not_copy {
public:
    haystack() { obstack_specify_allocation(&_, 0, 0, xmalloc, xfree); }
    ~haystack() { obstack_free(&_, NULL); }

    bool empty() const { return obstack_empty_p(&_); }

    long & chunk_size() { return obstack_chunk_size(&_); }
    int & alignment_mask() { return obstack_alignment_mask(&_); }
    int memory_used() { return obstack_memory_used(&_); }

    ::size_t alignment() {
        int a = alignment_mask() + 1;
        assert(0 == (a & (a-1)));
        return a;
    }

    void clear() {
        long cs = chunk_size();
        int am = alignment_mask();
        deallocate(NULL);
        obstack_specify_allocation(&_, cs, am, xmalloc, xfree);
    }

    void* allocate(::size_t s) { return obstack_alloc(&_, s); }

    template <typename T>
    T* allocate() {
        assert(alignof(T) <= alignment());
        return reinterpret_cast<T*>(allocate(sizeof(T)));
    }

    void deallocate(void* p) { return obstack_free(&_, p); }

    void* copy(const void* a, ::size_t s) { return obstack_copy(&_, a, s); }

    template <typename T>
    T* copy(const T &t) {
        assert(alignof(T) <= alignment());
        return reinterpret_cast<T*>(copy(&t, sizeof(t)));
    }

    void* copy0(const void* a, ::size_t s) { return obstack_copy0(&_, a, s); }

    template <typename T>
    T* copy0(const T &t) { return reinterpret_cast<T*>(copy0(&t, sizeof(t))); }

    void* base() const { return obstack_base(&_); }
    int size() const { return obstack_object_size(&_); }
    void* next() const { return obstack_next_free(&_); }
    int room() const { return obstack_room(&_); }

    void* finish() { return obstack_finish(&_); }
    void abort() { obstack_free(&_, finish()); }

    char* string() {
        int s = size();
        if (!s || reinterpret_cast<char*>(base())[s])
            zero();
        return reinterpret_cast<char*>(finish());
    }

    haystack& want(::size_t w) {
        obstack_make_room(&_, w);
        return *this;
    }

    void* blank(::size_t s = 0) {
        obstack_blank(&_, s);
        return obstack_next_free(&_) - s;
    }

    template <typename T>
    T* blank() {
        assert(alignof(T) <= alignment());
        return reinterpret_cast<T*>(blank(sizeof(T)));
    }

    haystack&
    shrink(::size_t s) {
        assert(s <= size());
        obstack_blank_fast(&_, -::ssize_t(s));
        return *this;
    }

    template <typename T>
    haystack&
    grow(const T* a, ::size_t s) {
        obstack_grow(&_, (const void*)a, s);
        return *this;
    }

    haystack& grow(const char * s) { return grow((void*)s, strlen(s)); }

    haystack&
    grow(const char c) {
        obstack_1grow(&_, c);
        return *this;
    }

    template <typename T>
    haystack& grow(const T &t) { return grow(&t, sizeof(t)); }

    haystack& pad(::size_t s) { blank(s); return *this; }
    haystack& zero() { return grow('\0'); }
    haystack& pointer(void* p) { obstack_ptr_grow(&_, p); return *this; }

    haystack&
    grow0(void* a, ::size_t s) {
        obstack_grow0(&_, a, s);
        return *this;
    }

    haystack& grow0(const char * s) { return grow0((void*)s, strlen(s)); }

    template <typename T>
    haystack& grow0(const T &t) { return grow0(&t, sizeof(t)); }

    haystack&
    printf(const char *f, ...) __attribute__ ((format (printf, 2, 3))) {
        va_list a;
        va_start(a, f);
        TRY(obstack_vprintf, &_, f, a);
        va_end(a);
        return *this;
    }

    haystack&
    vprintf(const char *f, va_list a) {
        TRY(obstack_vprintf, &_, f, a);
        return *this;
    }

    haystack& quote(const char *s) { return quote(s, strlen(s)); }

    haystack&
    quote(const char *s, ::size_t l) {
        while (l > 0) {
            if (::size_t o = strcspn(s, "'")) {
                want(l + 2 + 3);
                assert(room() >= o + 2);
                obstack_1grow_fast(&_, '\'');
                memcpy(next(), s, o);
                obstack_blank_fast(&_, o);
                obstack_1grow_fast(&_, '\'');
                s += o;
                l -= o;
            } else {
                want(l + 3);
            }
            if (l && *s) {
                assert(room() >= 3);
                obstack_1grow_fast(&_, '"');
                obstack_1grow_fast(&_, *s);
                obstack_1grow_fast(&_, '"');
                s += 1;
                l -= 1;
            } else {
                break;
            }
        }
        return *this;
    }

private:
    struct ::obstack _;
    static void * xmalloc(::size_t s) { return operator new(s); }
    static void xfree(void* p) { operator delete(p); }
};

} // namespace lace

#endif//LACE__HAYSTACK_H
