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

#include <NPR.h>
#include <libcdsTrees.h>

using namespace cds_utils;
using namespace cds_static;

NPR *saveLoad(NPR *bs) {
  std::ofstream ofs("npr.tmp");
  bs->save(ofs);
  ofs.close();
  std::ifstream ifs("npr.tmp");
  NPR *ret = NPR::load(ifs);
  ifs.close();
  return ret;
}

bool testNPR(NPR *npr, LCP *lcp, TextIndex *csa, size_t *naive_nsv,
             size_t *naive_psv, size_t l) {

  for (size_t i = 0; i < l; i++) {
    /*if(npr->find_NSV(i, csa,lcp) != naive_nsv[i]){
            std::cout << "Error in NSV" << std::endl;
            return false;
    }*/
    if (npr->find_PSV(csa->index_length() - l + i, csa, lcp) != naive_psv[i]) {
      std::cout << "Error in PSV" << std::endl;
      return false;
    }
  }

  int x, y, z, j, rmq_naive;
  size_t rmq_pos;
  for (size_t i = 0; i < l; i++) {
    x = (int)rand() % csa->index_length();
    y = (int)rand() % csa->index_length();
    if (y < x) {
      z = x;
      x = y;
      y = z;
    }
    rmq_naive = lcp->get_LCP(x, csa);
    rmq_pos = x;
    for (j = x + 1; j <= y; j++) {
      z = lcp->get_LCP(j, csa);
      if (z < rmq_naive) {
        rmq_naive = z;
        rmq_pos = j;
      }
    }
    if (rmq_pos != npr->find_RMQ(x, y, csa, lcp)) {
      std::cout << "Error in RMQ" << std::endl;
      return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  char *text;
  size_t length;
  LCP *lcp = NULL;
  NPR *npr = NULL;

  size_t *naive_nsv = NULL;
  size_t *naive_psv = NULL;

  if (argc != 2) {
    std::cout << "Checks if the NPR of the file <arch> is save/load correctly"
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

  lcp = new LCP_DAC(csa, text, length);

  size_t l = 1000;
  size_t aux_naive;
  int j;
  naive_nsv = new size_t[l];
  naive_psv = new size_t[l];

  std::cout << "Creating first " << l << " NSV_naive" << std::endl;
  for (size_t i = 0; i < l; i++) {
    aux_naive = lcp->get_LCP(i, csa);
    for (j = i + 1; (size_t)j < length; j++) {
      if (aux_naive > lcp->get_LCP(j, csa))
        break;
    }
    naive_nsv[i] = j;
  }

  std::cout << "Creating last " << l << " PSV_naive" << std::endl;
  for (size_t i = (length - l); i < length; i++) {
    aux_naive = lcp->get_LCP(i, csa);
    for (j = i - 1; j >= 0; j--) {
      if (aux_naive > lcp->get_LCP(j, csa))
        break;
    }
    naive_psv[i - (length - l)] = j + 1;
  }

  NPR_FMN nprFMN(lcp, 32, csa,
                 2); //(LCP, block_size, TextIndex, levels of recursion levels
                     // of recursion (try 1,2,3 or 4))
  npr = saveLoad(&nprFMN);
  if (!testNPR(npr, lcp, csa, naive_nsv, naive_psv, l)) {
    std::cerr << "ERROR TESTING NPR_FMN" << std::endl;
    return -1;
  }
  delete (NPR_FMN *)npr;
  std::cout << "NPR_FMN OK\n" << std::endl;

  NPR_CN nprCN(lcp, 32, csa); //(LCP, block_size, TextIndex)
  npr = saveLoad(&nprCN);
  if (!testNPR(npr, lcp, csa, naive_nsv, naive_psv, l)) {
    std::cerr << "ERROR TESTING NPR_CN" << std::endl;
    return -1;
  }
  delete (NPR_CN *)npr;
  std::cout << "NPR_CN OK\n" << std::endl;

  delete[] naive_nsv;
  delete[] naive_psv;
  delete (LCP_DAC *)lcp;
  delete (TextIndexCSA *)csa;
  delete[] text;
  return 0;
}
