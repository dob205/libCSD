
#include <cstdlib>

#include <BitSequence.h>
#include <Mapper.h>
#include <Sequence.h>
#include <libcdsBasics.h>

using namespace cds_static;

void testSequence(Array &a, Sequence &bs) {
  std::ofstream outfs("sequence.tmp");
  bs.save(outfs);
  outfs.close();
  std::ifstream infs("sequence.tmp");
  Sequence *seq = Sequence::load(infs);
  infs.close();
  uint maxv = a.getMax();
  size_t count[maxv + 1];
  for (size_t i = 0; i <= maxv; i++)
    count[i] = 0;
  for (size_t i = 0; i < a.getLength(); i++) {
    count[a[i]]++;
    for (uint j = a[i]; j <= a[i]; j++) {
      if (seq->rank(j, i) != count[j]) {
        std::cerr << "ERROR RANK " << std::endl;
        std::cerr << " Rank result: " << bs.rank(j, i) << " count=" << count[j]
                  << std::endl;
        std::cerr << " symbol=" << j << " position=" << i << std::endl;
        exit(-1);
      }
    }
    if (seq->select(a[i], count[a[i]]) != i) {
      std::cerr << "ERROR SELECT " << std::endl;
      std::cerr << "a[i]=" << a[i] << " maxv=" << maxv << std::endl;
      std::cerr << "bs.select=" << bs.select(a[i], count[a[i]]) << " i=" << i
                << std::endl;
      exit(-2);
    }
    if (a[i] != seq->access(i)) {
      std::cerr << "ERROR ACCESS" << std::endl;
      exit(-3);
    }
  }
  delete seq;
}

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
  }

  // BitmapsSequence  bs(a,new MapperNone(),new BitSequenceBuilderRRR(33));
  // testSequence(a, bs);

  Mapper *mapper = new MapperCont(a, BitSequenceBuilderRG(20));
  Mapper *mapper2 = new MapperNone();
  mapper->use();
  mapper2->use();
  std::cout << "Test 1 : Wavelet tree with pointers" << std::endl;
  WaveletTree wt1(a, new wt_coder_huff(a, mapper), new BitSequenceBuilderRG(20),
                  mapper);
  std::cout << "bs.size() = " << wt1.getSize() << std::endl;
  testSequence(a, wt1);

  std::cout << "Test 2 : Wavelet tree without pointers" << std::endl;
  WaveletTreeNoptrs wt3(a, new BitSequenceBuilderRRR(32), mapper);
  std::cout << "bs.size() = " << wt3.getSize() << std::endl;
  testSequence(a, wt3);
  mapper->unuse();
  mapper2->unuse();
  return 0;
}
