#ifndef LACE__ROPEBUF_H
#define LACE__ROPEBUF_H

#include "do_not_copy.h"

#include <ext/rope>
#include <string>
#include <streambuf>

namespace lace {

template < typename _RopeT = __gnu_cxx::rope<char>
         , typename _Traits = std::char_traits<typename _RopeT::value_type>
         >
class basic_ropebuf
    : public std::basic_streambuf<typename _RopeT::value_type, _Traits>
    , public lace::do_not_copy
    {
public:
    typedef _RopeT rope_type;
    typedef std::basic_streambuf<typename _RopeT::value_type, _Traits> streambuf_type;
    typedef typename streambuf_type::char_type char_type;
    typedef typename streambuf_type::traits_type traits_type;
    typedef typename streambuf_type::int_type int_type;
    typedef typename streambuf_type::pos_type pos_type;
    typedef typename streambuf_type::off_type off_type;

    basic_ropebuf() { }
    basic_ropebuf(const rope_type &r) : _(r) { }

    rope_type & rope() { return _; }
    const rope_type & rope() const { return _; }

    std::streamsize showmanyc() { return _.size(); }

    std::streamsize
    xsgetn(char_type *s, std::streamsize n) {
        typename rope_type::size_type l = _.copy(0, n, s);
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
