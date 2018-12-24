#include "hash.h"
#include <cassert>
#include <cstdlib>
#include <string>
#include <algorithm>

int main(int, char*[]) {
  assert(lace::hash(0) == lace::hash(0));

  assert(lace::hash<uint8_t>(1) == lace::hash<uint8_t>(1));
  assert(lace::hash<uint16_t>(1) == lace::hash<uint16_t>(1));
  assert(lace::hash<uint32_t>(1) == lace::hash<uint32_t>(1));
  assert(lace::hash<uint64_t>(1) == lace::hash<uint64_t>(1));

  assert(lace::hash<uint8_t>(1) != lace::hash<uint16_t>(1));
  assert(lace::hash<uint16_t>(1) != lace::hash<uint32_t>(1));
  assert(lace::hash<uint32_t>(1) != lace::hash<uint64_t>(1));

  assert(lace::hash<uint8_t>(1)  != lace::hash<uint8_t>(2));
  assert(lace::hash<uint16_t>(1) != lace::hash<uint16_t>(2));
  assert(lace::hash<uint32_t>(1) != lace::hash<uint32_t>(2));
  assert(lace::hash<uint64_t>(1) != lace::hash<uint64_t>(2));

  assert(lace::hash<const char *>("a") == lace::hash<const char *>("a"));
  assert(lace::hash<const char *>("a") != lace::hash<const char *>("b"));

  char c[] = "a";
  assert(lace::hash<const char *>("a") == lace::hash<char *>(c));

  assert(lace::hash("a") == lace::hash("a"));
  assert(lace::hash(L"a") == lace::hash(L"a"));
  assert(lace::hash(L"a") != lace::hash("a"));

  assert(lace::hash<const char *>("a") == lace::hash(std::string("a")));
  assert(lace::hash<const char *>("a") != lace::hash(std::string("b")));

  assert(lace::hash<std::string>(std::string("a")) == lace::hash<const std::string>(std::string("a")));
  assert(lace::hash<std::string>(std::string("a")) != lace::hash<std::string>(("b")));

  std::string A("A");
  std::string aa("aa");
  std::transform(A.begin(), A.end(), A.begin(), ::tolower);
  assert(lace::hash(A) == lace::hash(aa.substr(1)));

  return EXIT_SUCCESS;
}
