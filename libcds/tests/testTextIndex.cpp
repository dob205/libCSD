
#include <cstdlib>

#include <TextIndex.h>
#include <libcdsTrees.h>


using namespace cds_utils;
using namespace cds_static;

TextIndex *saveLoad(TextIndex *bs) {
  std::ofstream ofs("textindex.tmp");
  bs->save(ofs);
  ofs.close();
  std::ifstream ifs("textindex.tmp");
  TextIndex *ret = TextIndex::load(ifs);
  ifs.close();
  return ret;
}

bool testTextIndex(TextIndex *s1, TextIndex *s2) {
  for (size_t i = 0; i < s2->index_length(); i++) {
    if (s1->getSA(i) != s2->getSA(i))
      return false;
    if (s1->getISA(i) != s2->getISA(i))
      return false;
    if (s1->getPsi(i) != s2->getPsi(i))
      return false;
  }
  return true;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    std::cout << "Checks if the TextIndex of the file <arch> is save/load correctly"
         << endl
         << std::endl;
    std::cout << "usage: " << argv[0] << " <arch>" << std::endl;
    return 0;
  }
  char *text = NULL;
  size_t length;
  TextIndex *ticsa, *load_ticsa;

  if (loadText(argv[1], &text, &length))
    return 1;
  std::cout << "length: " << length << std::endl;

  ticsa = new TextIndexCSA((uchar *)text, (ulong)length, NULL);

  load_ticsa = saveLoad(ticsa);

  if (!testTextIndex(ticsa, load_ticsa)) {
    std::cerr << "ERROR TESTING TextIndexCSA" << std::endl;
    return -1;
  }

  std::cout << "CSA OK\n" << std::endl;
  delete (TextIndexCSA *)ticsa;
  delete (TextIndexCSA *)load_ticsa;
  if (text != NULL)
    delete text;
}
