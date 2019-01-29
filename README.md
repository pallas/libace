libace
======

libace is a collection of C++ code that the author finds himself
copy-and-pasting quite often.

Includes

 * compare --- Generic less-than, equal, greater-than template.
 * divider --- Implements division-by-constant as multiplication.
 * do_not_copy --- Equivalent to boost::noncopyable.
 * hash --- Generic hash template, implemented as 64-bit FNV-1a.
 * haystack -- Wrapper around obstack.
 * objectstack --- A haystack that understands destructors.
 * ropebuf --- Glue between an SGI rope and std::streambuf.
 * singleton --- Singleton template, instantiates before main.
 * try --- Converts traditional error reporting to exceptions.
