
#include <cstdlib>

#include <Coder.h>
#include <libcdsBasics.h>

using namespace cds_utils;
using namespace cds_static;

int main(int argc, char **argv) {

  if (argc != 4) {
    std::cout
        << "Checks the array class generating <length> elements between 0 and "
           "<maxv> using <seed> as seed for the numbers generation"
        << endl
        << std::endl;
    std::cout << "usage: " << argv[0] << " <seed> <length> <maxv>" << std::endl;
    return 0;
  }

  srand(transform(string(argv[1])));
  uint len = transform(string(argv[2]));
  uint maxv = transform(string(argv[3]));

  // std::cout << "maxv = " << maxv << std::endl;
  // std::cout << "len  = " << len << std::endl;

  Array a(len, maxv);
  for (uint i = 0; i < len; i++) {
    a.setField(i, rand() % maxv);
    std::cout << "s[" << i << "]=" << a[i] << std::endl;
  }

  HuffmanCoder *hc = new HuffmanCoder(a);
  std::cout << "Max length: " << hc->maxLength() << std::endl;

  for (size_t i = 0; i < len; i++) {
    size_t ptr = 0;
    uint stream[10];
    std::cout << "coding " << a[i] << ": ";
    ptr = hc->encode(a[i], stream, ptr);
    for (size_t k = 0; k < ptr; k++)
      std::cout << bitget(stream, k);
    std::cout << std::endl;
  }
  return 0;
}
