/* Build.cpp
 * Copyright (C) 2014, Francisco Claude & Rodrigo Canovas & Miguel A.
 * Martinez-Prieto all rights reserved.
 *
 * Script for building indexed string dictionaries in compressed space.
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
 *
 * Contacting the authors:
 *   Francisco Claude:  	fclaude@recoded.cl
 *   Rodrigo Canovas:  		rcanovas@student.unimelb.edu.au
 *   Miguel A. Martinez-Prieto:	migumar2@infor.uva.es
 */

#ifndef _BUILD_CPP
#define _BUILD_CPP

#include <fstream>
#include <iostream>

#include "StringDictionary.h"
#include "iterators/IteratorDictStringPlain.h"
#include "utils/LogSequence.h"

void checkDict(void);
void checkFile(void);
void useBuild(void);

void checkDict() {
  std::cerr << std::endl;
  std::cerr
      << " *******************************************************************"
         "************* "
      << std::endl;
  std::cerr
      << " *** Checks the given file because it does not represent any valid "
         "dictionary *** "
      << std::endl;
  std::cerr
      << " *******************************************************************"
         "************* "
      << std::endl;
  std::cerr << std::endl;
}

void checkFile() {
  std::cerr << std::endl;
  std::cerr
      << " ****************************************************************** "
      << std::endl;
  std::cerr
      << " *** Checks the given path because it does not contain any file *** "
      << std::endl;
  std::cerr
      << " ****************************************************************** "
      << std::endl;
  std::cerr << std::endl;
}

void useBuild() {

  std::cerr << std::endl;
  std::cerr
      << " *******************************************************************"
         "******* "
      << std::endl;
  std::cerr
      << " *** BUILD script for indexing string dictionaries in compressed "
         "space. *** "
      << std::endl;
  std::cerr
      << " *******************************************************************"
         "******* "
      << std::endl;
  std::cerr << std::endl;
  std::cerr << " ----- ./Build <type> <parameters> <in> <out>" << std::endl;

  std::cerr << " type: 1 => Build HASH dictionary" << std::endl;
  std::cerr << " \t <compress> : 'h' for Huffman; 'r' for RePair compression."
            << std::endl;
  std::cerr
      << " \t <overhead> : hash table overhead over the number of strings."
      << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;

  std::cerr << " type: 2 => Build HASHDAC dictionary" << std::endl;
  std::cerr << " \t <compress> : 'h' for Huffman; 'r' for RePair compression."
            << std::endl;
  std::cerr
      << " \t <overhead> : hash table overhead over the number of strings."
      << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;

  std::cerr << " type: 3 => Build PLAIN FRONT CODING dictionary" << std::endl;
  std::cerr
      << " \t <compress> : 'p' for plain (uncompressed) representation; 'r' "
         "for RePair compression"
      << std::endl;
  std::cerr << " \t <bucketsize> : number of strings per bucket." << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;

  std::cerr << " type: 4 => Build HU-TUCKER FRONT CODING dictionary"
            << std::endl;
  std::cerr << " \t <compress> : tecnique used for internal string compression."
            << std::endl;
  std::cerr
      << " \t              't' for HuTucker; 'h' for Huffman; 'r' for RePair "
         "compression."
      << std::endl;
  std::cerr << " \t <bucketsize> : number of strings per bucket." << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;

  std::cerr << " type: 5 => Build RE-PAIR DAC dictionary" << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;

  std::cerr << " type: 6 => Build FMINDEX dictionary" << std::endl;
  std::cerr
      << " \t <compress> : 'p' for plain bitmaps; 'c' for compresed bitmaps."
      << std::endl;
  std::cerr << " \t <BS sampling> : sampling value for the bitmaps"
            << std::endl;
  std::cerr
      << " \t <BWT sampling> : sampling step for the BWT (0 for no sampling)"
      << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;

  std::cerr << " type: 7 => Build XBW dictionary" << std::endl;
  std::cerr
      << " \t <in> : input file containing the set of '\\0'-delimited strings."
      << std::endl;
  std::cerr << " \t <out> : output file for storing the dictionary."
            << std::endl;
  std::cerr << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int type = atoi(argv[1]);

    switch (type) {
    case 1: {
      if (argc != 6) {
        useBuild();
        break;
      }

      std::ifstream in(argv[4]);

      if (in.good()) {

        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uint overhead = atoi(argv[3]);

        uchar *str = loadValue<uchar>(in, lenStr + 1);
        str[lenStr] = '\0';
        in.close();

        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
        StringDictionary *dict = NULL;
        std::string filename = std::string(argv[5]);

        if (argv[2][0] == 'h') {
          // Huffman compression
          dict = new StringDictionaryHASHHF(it, lenStr, overhead);
          filename += std::string(".hashhf");
        } else {
          // RePair compression
          dict = new StringDictionaryHASHRPF(it, lenStr, overhead);
          filename += std::string(".hashrpf");
        }

        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    case 2: {
      if (argc != 6) {
        useBuild();
        break;
      }

      std::ifstream in(argv[4]);

      if (in.good()) {
        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uint overhead = atoi(argv[3]);

        uchar *str = loadValue<uchar>(in, lenStr + 1);
        str[lenStr] = '\0';
        in.close();

        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
        StringDictionary *dict = NULL;
        std::string filename = std::string(argv[5]);

        if (argv[2][0] == 'h') {
          // Huffman compression
          dict = new StringDictionaryHASHUFFDAC(it, lenStr, overhead);
          filename += std::string(".hashuffdac");
        } else {
          // RePair compression
          dict = new StringDictionaryHASHRPDAC(it, lenStr, overhead);
          filename += std::string(".hashrpdac");
        }

        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    case 3: {
      if (argc != 6) {
        useBuild();
        break;
      }

      std::ifstream in(argv[4]);
      if (in.good()) {
        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uint bucketsize = atoi(argv[3]);

        uchar *str = loadValue<uchar>(in, lenStr);
        in.close();

        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
        StringDictionary *dict = NULL;
        std::string filename = std::string(argv[5]);

        if (argv[2][0] == 'p') {
          // Uncompressed internal string
          dict = new StringDictionaryPFC(it, bucketsize);
          filename += std::string(".pfc");
        } else if (argv[2][0] == 'r') {
          // RePair compression
          dict = new StringDictionaryRPFC(it, bucketsize);
          filename += std::string(".rpfc");
        } else {
          useBuild();
          delete it;
          return 0;
        }

        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    case 4: {
      if (argc != 6) {
        useBuild();
        break;
      }

      std::ifstream in(argv[4]);
      if (in.good()) {
        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uint bucketsize = atoi(argv[3]);

        uchar *str = loadValue<uchar>(in, lenStr);
        in.close();

        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
        StringDictionary *dict = NULL;
        std::string filename = std::string(argv[5]);

        switch (argv[2][0]) {
        case 't': {
          // HuTucker compression
          dict = new StringDictionaryHTFC(it, bucketsize);
          filename += std::string(".htfc");
          break;
        }

        case 'h': {
          // Huffman compression
          dict = new StringDictionaryHHTFC(it, bucketsize);
          filename += (".hhtfc");
          break;
        }

        case 'r': {
          // RePair compression
          dict = new StringDictionaryRPHTFC(it, bucketsize);
          filename += std::string(".rphtfc");
          break;
        }

        default: {
          useBuild();
          delete it;
          return 0;
        }
        }

        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    case 5: {
      if (argc != 4) {
        useBuild();
        break;
      }

      std::ifstream in(argv[2]);
      if (in.good()) {
        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uchar *str = loadValue<uchar>(in, lenStr);
        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
        in.close();

        StringDictionary *dict = new StringDictionaryRPDAC(it);
        std::string filename = std::string(argv[3]) + std::string(".rpdac");

        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    case 6: {
      if (argc != 7) {
        useBuild();
        break;
      }

      std::ifstream in(argv[5]);
      if (in.good()) {
        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uchar *str = loadValue<uchar>(in, lenStr);
        in.close();

        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
        StringDictionary *dict = NULL;
        std::string filename;

        uint BWTsampling = atoi(argv[4]);

        if (argv[2][0] == 'p') {
          // Plain bitmaps
          dict = new StringDictionaryFMINDEX(it, false, atoi(argv[3]),
                                             BWTsampling);
          delete it;

          filename = std::string(argv[6]) + std::string(".") +
                     std::string(argv[3]) + std::string(".rg.fmi");

        } else if (argv[2][0] == 'c') {
          // Compressed bitmaps
          dict =
              new StringDictionaryFMINDEX(it, true, atoi(argv[3]), BWTsampling);
          delete it;

          filename = std::string(argv[6]) + std::string(".") +
                     std::string(argv[3]) + std::string(".rrr.fmi");
        } else {
          checkFile();
          break;
        }

        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    case 7: {
      if (argc != 4) {
        useBuild();
        break;
      }

      std::ifstream in(argv[2]);
      if (in.good()) {
        in.seekg(0, std::ios_base::end);
        uint lenStr = in.tellg() / sizeof(uchar);
        in.seekg(0, std::ios_base::beg);

        uchar *str = loadValue<uchar>(in, lenStr);
        IteratorDictString *it = new IteratorDictStringPlain(str, lenStr - 1);
        in.close();

        StringDictionary *dict = new StringDictionaryXBW(it);
        delete it;

        std::string filename = std::string(argv[3]) + std::string(".xbw");
        std::ofstream out((char *)filename.c_str());
        dict->save(out);
        out.close();
        delete dict;
      } else
        checkFile();

      break;
    }

    default: {
      useBuild();
      break;
    }
    }
  } else {
    useBuild();
  }
}

#endif /* _BUILD_CPP */
