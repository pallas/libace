#include "random.h"
#include "singleton.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

lace::random & rng = lace::singleton<lace::random>::instance();

template<typename T>
T & fisher_yates(T & a) {
    for (unsigned i = a.size() - 1 ; i > 0 ; --i)
        std::swap(a[i], a[rng.roll(i)]);
    return a;
}

int
main(int argc, char* argv[]) {
    std::vector<std::string> cards = {
        "♠A", "♠K", "♠Q", "♠J", "♠T", "♠9", "♠8", "♠7", "♠6", "♠5", "♠4", "♠3", "♠2",
        "♥A", "♥K", "♥Q", "♥J", "♥T", "♥9", "♥8", "♥7", "♥6", "♥5", "♥4", "♥3", "♥2",
        "♦A", "♦K", "♦Q", "♦J", "♦T", "♦9", "♦8", "♦7", "♦6", "♦5", "♦4", "♦3", "♦2",
        "♣A", "♣K", "♣Q", "♣J", "♣T", "♣9", "♣8", "♣7", "♣6", "♣5", "♣4", "♣3", "♣2",
    };

    fisher_yates(cards);

    for (auto i = cards.begin() ; i < cards.end() ; i += 13) {
        std::sort(i, i + 13);
        for (auto j = i ; j < i + 13 ; ++j)
            std::cout << " "[i==j] << *j;
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

//
