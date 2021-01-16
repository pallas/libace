#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>

#include "objectstack.h"

struct x {
  x(int ii = 0) : i(ii) { }
  int i;
};

struct A {
  A() : c(new char) { std::cerr << "+A " << this << std::endl; }
  virtual ~A() { std::cerr << "-A " << this << std::endl; delete c; }
  char * c;
};

struct B : virtual public A {
  B() { std::cerr << "+B " << this << std::endl; }
  virtual ~B() { std::cerr << "-B " << this << std::endl; }
};

struct C : virtual public A {
  C() { std::cerr << "+C " << this << std::endl; }
  virtual ~C() { std::cerr << "-C " << this << std::endl; }
};

struct D : virtual public B, virtual public C {
  D(int ii = 0) : i(ii) { std::cerr << "+D " << this << std::endl; }
  virtual ~D() { std::cerr << "-D " << this << std::endl; }
  int i;
};

struct E : public A { };

struct T {
  T() { throw std::runtime_error("whoops"); }
};


int main(int, char*[]) {
  lace::objectstack os;

  auto x1 = os.make<x>(1);
  auto d1 = os.make<D>(1);
  try { auto t = os.make<T>(); } catch(...) { std::cerr << "catch" << std::endl; }
  auto e = os.make<E>();
  auto d = os.make<D>();
  auto c = os.make<C>();
  auto b = os.make<B>();
  auto a = os.make<A>();
  auto x0 = os.make<x>();
  std::cerr << *os.make<std::string>() << std::endl;

  return EXIT_SUCCESS;
}
