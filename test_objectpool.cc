#include <cstdlib>
#include <iostream>
#include <string>

#include "objectpool.h"

struct make_noise {
  make_noise() { std::cerr << "+" << this << std::endl; }
  ~make_noise() { std::cerr << "-" << this << std::endl; }
};

class noisy_string : public std::string, private make_noise {
public:
  noisy_string() { }
  noisy_string(const std::string& str) : std::string(str) { }
  noisy_string(const std::string& str, std::size_t pos, std::size_t len = std::string::npos) : std::string(str,pos,len) { }
  noisy_string(const char* s) : std::string(s) { }
  noisy_string(const char* s, std::size_t n) : std::string(s,n) { }
  noisy_string(std::size_t n, char c) : std::string(n,c) { }

  template <class InputIterator>
    noisy_string(InputIterator first, InputIterator last) : std::string(first,last) { }

  noisy_string(std::initializer_list<char> il) : std::string(il) { }
  noisy_string(std::string&& str) noexcept : std::string(str) { }
};

int main(int, char*[]) {
  {{
    lace::objectpool<noisy_string> op;

    noisy_string ns("hi");
    noisy_string *nxm = op.allocate("xyzzy");
    std::cerr << *nxm << std::endl;
    noisy_string *nx10 = op.allocate(10,'1');
    std::cerr << *nx10 << std::endl;
    noisy_string *nx20 = op.allocate(20,'2');
    std::cerr << *nx20 << std::endl;
    op.deallocate(nx10);
    noisy_string *nx30 = op.allocate(30,'3');
    std::cerr << *nx30 << std::endl;
    noisy_string *nx40 = op.allocate(40,'4');
    std::cerr << *nx40 << std::endl;
    op.deallocate(nx20);
    op.deallocate(nx30);
    noisy_string *nx50 = op.allocate(50,'5');
    std::cerr << *nx50 << std::endl;
    noisy_string *nx60 = op.allocate(60,'6');
    std::cerr << *nx60 << std::endl;
    op.deallocate(nx50);
    op.deallocate(nx40);
  }}

  {{
    lace::objectpool<long long> op;
    for (int i = 0 ; i < 10 ; ++i) {
      auto x = op.allocate(i);
      auto y = op.allocate(1<<*x);
      op.deallocate(x);
      auto z = op.allocate(*y - 1);
      op.deallocate(y);
    }
  }}

  return EXIT_SUCCESS;
}
