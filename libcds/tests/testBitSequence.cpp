
#include <cstdlib>

#include <BitSequence.h>
#include <BitSequenceRG.h>
#include <BitSequenceRRR.h>
#include <BitSequenceSDArray.h>
#include <BitString.h>
#include <libcdsBasics.h>

using namespace cds_utils;
using namespace cds_static;

BitSequence *saveLoad(BitSequence *bs) {
  std::ofstream ofs("bitsequence.tmp");
  bs->save(ofs);
  ofs.close();
  std::ifstream ifs("bitsequence.tmp");
  BitSequence *ret = BitSequence::load(ifs);
  ifs.close();
  return ret;
}

bool testBitSequence(BitString &a, BitSequence *bs) {
  size_t rank0SoFar = 0;
  size_t rank1SoFar = 0;
  for (size_t i = 0; i < a.getLength(); i++) {
    if (a[i]) {
      rank1SoFar++;
      if (bs->select1(rank1SoFar) != i) {
        std::cerr << "SELECT1 ERROR " << i << std::endl;
        return false;
      }
      if (i > 0 && bs->selectNext1(i) != i) {
        std::cout << "i=" << i << "sn=" << bs->selectNext1(i) << std::endl;
        std::cerr << "SELECTNEXT1 ERROR" << std::endl;
        return false;
      }
    } else {
      rank0SoFar++;
      if (bs->select0(rank0SoFar) != i) {
        std::cerr << "SELECT0 ERROR" << std::endl;
        return false;
      }
      if (i > 0 && bs->selectNext0(i) != i) {
        std::cerr << "SELECTNEXT0 ERROR" << std::endl;
        return false;
      }
    }
    if (bs->rank1(i) != rank1SoFar)
      return false;
    if (bs->rank0(i) != rank0SoFar)
      return false;
    if (bs->access(i) != a[i])
      return false;
  }
  return true;
}

int main(int argc, char **argv) {

  if (argc != 4) {
    std::cout
        << "Checks the bitsequence classes generating <length> bitmaps with "
           "density <dens> using <seed> as seed for the numbers generation"
        << endl
        << std::endl;
    std::cout << "usage: " << argv[0] << " <seed> <length> <dens>" << std::endl;
    return 0;
  }

  srand(transform(string(argv[1])));
  uint len = transform(string(argv[2]));
  uint dens = transform(string(argv[3]));

  uint act_dens = 0;
  BitString a(len);
  while (act_dens < dens) {
    size_t pos = rand() % len;
    if (!a[pos]) {
      act_dens++;
      a.setBit(pos, true);
    }
  }

  BitSequenceRG bsRG(a, 20);
  BitSequence *s = saveLoad(&bsRG);
  if (!testBitSequence(a, s)) {
    std::cerr << "ERROR TESTING BitSequenceRG" << std::endl;
    return -1;
  }
  std::cout << "RG OK\n" << std::endl;
  delete (BitSequenceRG *)s;

  BitSequenceDArray bsDArray(a);
  s = saveLoad(&bsDArray);
  if (!testBitSequence(a, s)) {
    std::cerr << "ERROR TESTING BitSequenceDArray" << std::endl;
    return -1;
  }
  std::cout << "DArray OK\n" << std::endl;
  delete (BitSequenceDArray *)s;

  BitSequenceSDArray bsSDArray(a);
  s = saveLoad(&bsSDArray);
  if (!testBitSequence(a, s)) {
    std::cerr << "ERROR TESTING BitSequenceSDArray" << std::endl;
    return -1;
  }
  std::cout << "SDArray OK\n" << std::endl;
  delete (BitSequenceSDArray *)s;

  BitSequenceRRR bsRRR(a, 33);
  s = saveLoad(&bsRRR);
  if (!testBitSequence(a, s)) {
    std::cerr << "ERROR TESTING BitSequenceRRR" << std::endl;
    return -1;
  }
  std::cout << "RRR OK\n" << std::endl;
  delete (BitSequenceRRR *)s;
  return 0;
}
