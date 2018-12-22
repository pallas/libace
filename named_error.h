#ifndef LACE__NAMED_ERROR_HH
#define LACE__NAMED_ERROR_HH

#include <stdexcept>
#include <typeinfo>

#define NAMED_ERROR(x) \
    class x : public std::runtime_error { public: \
        x() : std::runtime_error(typeid(*this).name()) { } \
    }

#endif//LACE__NAMED_ERROR_HH
