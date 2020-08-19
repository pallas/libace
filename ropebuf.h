#ifndef LACE__ROPEBUF_H
#define LACE__ROPEBUF_H

#include "do_not_copy.h"

#include <algorithm>
#include <ext/rope>
#include <memory>
#include <streambuf>
#include <string>

namespace lace {

typedef __gnu_cxx::rope<char> rope;

template < typename _RopeT = rope
         , typename _Traits = std::char_traits<typename _RopeT::value_type>
         , typename _Allocator = std::allocator<typename _RopeT::value_type>
         >
class basic_ropebuf
    : public std::basic_streambuf<typename _RopeT::value_type, _Traits>
    , public lace::do_not_copy
    {
public:
    typedef _RopeT rope_type;
    typedef typename rope_type::size_type size_type;
    typedef std::basic_streambuf<typename _RopeT::value_type, _Traits> streambuf_type;
    typedef typename streambuf_type::char_type char_type;
    typedef typename streambuf_type::traits_type traits_type;
    typedef typename streambuf_type::int_type int_type;
    typedef typename streambuf_type::pos_type pos_type;
    typedef typename streambuf_type::off_type off_type;
    typedef __gnu_cxx::_Rope_char_consumer<char_type> consumer_type;
    typedef __gnu_cxx::char_producer<char_type> producer_type;
    typedef std::basic_string<char_type, traits_type, _Allocator> string_type;

    basic_ropebuf() { }
    basic_ropebuf(const rope_type &r) : _(r) { }

    rope_type & rope() { return _; }
    const rope_type & rope() const { return _; }

private:
    class consume_to_string : public consumer_type {
    public:
        consume_to_string(string_type &_) : s(_) { }
        bool operator() (const char_type * buffer, size_type length) {
            s.append(buffer, length);
            return true;
        }

    private:
        string_type &s;
    };

public:
    string_type string() const { return string(_.size()); }
    string_type string(size_t length) const { return string(0, length); }

    string_type
    string(size_type offset, size_type length) const {
        string_type s;
        size_type available = _.size();
        if (offset < available) {
            consume_to_string cts(s);
            if (length > available - offset)
                length = available - offset;
            s.reserve(length);
            _.apply_to_pieces(offset, length, cts);
        }
        return s;
    }

    std::streamsize showmanyc() { return _.size(); }

    std::streamsize
    xsgetn(char_type *s, std::streamsize n) {
        size_type l = _.copy(0, n, s);
        _.erase(0, n);
        return l;
    }

    int_type underflow() { return _.empty() ? traits_type::eof() : _.front(); }

    int_type
    uflow() {
        if (_.empty())
            return traits_type::eof();

        int_type i = underflow();
        _.pop_front();
        return i;
    }

    int_type
    pbackfail(int_type c = traits_type::eof()) {
        if (traits_type::eof() != c)
            _.insert(0, c);
        return c;
    }

    std::streamsize
    xsputn(const char_type *s, std::streamsize n) {
        _.append(s, n);
        return n;
    }

    int_type
    overflow(int_type c = traits_type::eof()) {
        if (traits_type::eof() != c)
            _.append(c);
        return c;
    }

private:
    rope_type _;
};

typedef basic_ropebuf<> ropebuf;

} // namespace lace

#endif//LACE__ROPEBUF_H
