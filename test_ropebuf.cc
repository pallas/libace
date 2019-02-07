#include "ropebuf.h"

#include <string>
#include <cstdlib>
#include <iostream>

int main(int, char*[]) {
    lace::ropebuf rs;
    std::ostream os(&rs);
    for (const char * c : { "x", "y", "z", "z", "y" } )
        os << c << ' ';

    auto &r = rs.rope();
    r.append(42, '*');
    r.balance();

    std::istream is(&rs);
    std::string s;
    while (is >> s)
        std::cout << '|' << s << '|' << std::endl;

    return EXIT_SUCCESS;
}
