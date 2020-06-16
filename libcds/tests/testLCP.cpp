/* Copyright (C) 2010, Rodrigo Cánovas, all rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <LCP.h>
#include <TextIndex.h>
#include <libcdsTrees.h>


using namespace cds_utils;
using namespace cds_static;

LCP *saveLoad(LCP *bs) {
  std::ofstream ofs("lcp.tmp");
  bs->save(ofs);
  ofs.close();
  std::ifstream ifs("lcp.tmp");
  LCP *ret = LCP::load(ifs);
  ifs.close();
  return ret;
}

bool testLCP(LCP *s1, LCP *s2, TextIndex *csa) {
  for (size_t i = 0; i < csa->index_length(); i++) {
    if (s1->get_LCP(i, csa) != s2->get_LCP(i, csa))
      return false;
  }
  size_t *next;
  size_t n_next = 0;
  for (size_t i = 0; i < csa->index_length(); i++) {
    if ((i % 100) == 0)
      n_next = 0;
    if (s1->get_seq_LCP(i, csa, &next, &n_next, 1) !=
        s2->get_seq_LCP(i, csa, &next, &n_next, 1))
      return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  char *text;
  size_t length;
  LCP *lcp_naive = NULL;
  LCP *lcp = NULL;

  if (argc != 2) {
    std::cout << "Checks if the LCP of the file <arch> is save/load correctly"
         << endl
         << std::endl;
    std::cout << "usage: " << argv[0] << " <arch>" << std::endl;
    return 0;
  }

  TextIndex *csa;

  if (loadText(argv[1], &text, &length))
    return 1;

  std::cout << "length: " << length << std::endl;

  /*create index*/
  csa = new TextIndexCSA((uchar *)text, (ulong)length, NULL);

  lcp_naive = new LCP_naive(csa, text, length);

  lcp = saveLoad(lcp_naive);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_naive" << std::endl;
    return -1;
  }
  delete (LCP_naive *)lcp;
  std::cout << "LCP_naive OK\n" << std::endl;

  LCP_Sad lcpSad(csa, text, length);
  lcp = saveLoad(&lcpSad);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_Sad" << std::endl;
    return -1;
  }
  delete (LCP_Sad *)lcp;
  std::cout << "LCP_Sad OK\n" << std::endl;

  LCP_FMN lcpFMN(csa, text, length);
  lcp = saveLoad(&lcpFMN);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_FMN" << std::endl;
    return -1;
  }
  delete (LCP_FMN *)lcp;
  std::cout << "LCP_FMN OK\n" << std::endl;

  LCP_PT lcpPT(csa, text, length,
               6); // the last parameter can be 1,2,3,4,5,6,7,8
  lcp = saveLoad(&lcpPT);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_PT" << std::endl;
    return -1;
  }
  delete (LCP_PT *)lcp;
  std::cout << "LCP_PT OK\n" << std::endl;

  LCP_PhiSpare lcpPhi(csa, text, length,
                      8); // the last parameter can be -1, 0, ..., length-1
  lcp = saveLoad(&lcpPhi);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_PhiSpare" << std::endl;
    return -1;
  }
  delete (LCP_PhiSpare *)lcp;
  std::cout << "LCP_PhiSpare OK\n" << std::endl;

  LCP_DAC lcpDAC(csa, text, length);
  lcp = saveLoad(&lcpDAC);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_DAC" << std::endl;
    return -1;
  }
  delete (LCP_DAC *)lcp;
  std::cout << "LCP_DAC OK\n" << std::endl;

  LCP_DAC_VAR lcpDACVAR(csa, text, length);
  lcp = saveLoad(&lcpDACVAR);
  if (!testLCP(lcp_naive, lcp, csa)) {
    std::cerr << "ERROR TESTING LCP_DAC_VAR" << std::endl;
    return -1;
  }
  delete (LCP_DAC_VAR *)lcp;
  std::cout << "LCP_DAC_VAR OK\n" << std::endl;

  delete (LCP_naive *)lcp_naive;
  delete (TextIndexCSA *)csa;
  delete[] text;
  return 0;
}
