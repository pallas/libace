#include "haystack.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <arpa/inet.h>

#include <iostream>

char*
expect(const char* want, void* have) {
  assert(0 == memcmp(have, want, 1+strlen(want)));
  return (char*)have;
}

int main(int, char*[]) {
  lace::haystack os;

  std::cout
      << expect("abcxyz", os.copy0("abcxyz")) << std::endl
      << expect("abcxyz", os.grow('a').grow('b').grow('c').grow("xyz").grow('\0').pad(os.room()).finish()) << std::endl
      << expect("abcxyz", os.grow("abc").grow0("xyz").pad(os.room()).finish()) << std::endl
      << expect("abcxyz", os.grow0("abcxyz").grow("xyz").pad(os.room()).finish()) << std::endl
      << expect("abcxyz", os.grow0("abc").shrink(1).grow("xyz").zero().pad(os.room()).finish()) << std::endl
      << expect("abcxyz", os.grow(htonl(0x61626300)).shrink(1).grow0("xyz").pad(os.room()).finish()) << std::endl
      << expect("abcxyz", os.printf("%s%s", "abc", "xyz").zero().pad(os.room()).finish()) << std::endl;
  os.clear();

  std::cout
      << expect("\"''\"'\"\"'\"''\"", os.quote("''\"\"''").string()) << std::endl
      << expect("'\"\"'\"''\"'\"\"'", os.quote("\"\"''\"\"").string()) << std::endl
      << expect("\"'\"'\"'\"'\"", os.quote("'\"'").string()) << std::endl
      << expect("'\"'\"'\"'\"'", os.quote("\"'\"").string()) << std::endl
      << expect("\"'\"", os.quote("'").string()) << std::endl
      << expect("'\"'", os.quote("\"").string()) << std::endl
      << expect("", os.quote("").string()) << std::endl
      ;
  os.clear();

  os.pointer("xyzzy").finish();
  const char evil[] = "'\0'";
  std::cout << expect("\"'\"", os.quote(evil, sizeof(evil)).string()) << std::endl;
  os.clear();

  return EXIT_SUCCESS;
}
