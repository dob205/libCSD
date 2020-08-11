#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "StringDictionaryHASHRPDACBlocks.h"

char *get_plain(std::vector<std::string> &data, size_t &total_size) {
  total_size = 0;
  for (auto &s : data) {
    total_size += s.size() + 1;
  }
  auto *buf = new char[total_size];
  size_t curr = 0;
  for (auto &s : data) {
    memcpy(buf + curr, s.data(), s.size() + 1);
    curr += s.size() + 1;
  }

  return buf;
}

TEST(StringDictionaryHASHRPDACBlocksTests, can_serialize) {
  std::vector<std::string> data;

  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAC");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAE");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAF");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAI");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJ");

  std::sort(data.begin(), data.end());

  for (size_t j = 1; j <= 29; j++) {
    size_t total_size = 0;
    auto *plain_data = get_plain(data, total_size);

    auto *it = new IteratorDictStringPlain(
        reinterpret_cast<unsigned char *>(plain_data), total_size);

    StringDictionaryHASHRPDACBlocks sd(it, total_size, 25, 1 << j);

    std::string filename =
        "StringDictionaryHASHRPDACBlocksTests_can_serialize.bin";

    {
      std::ofstream ofs(filename, std::ios::out | std::ios::binary);
      sd.save(ofs);
    }

    StringDictionary *deserialized;
    {
      std::ifstream ifs(filename, std::ios::in | std::ios::binary);
      deserialized = StringDictionaryHASHRPDACBlocks::load(ifs);
    }

    for (auto &s : data) {
      auto i = deserialized->locate(
          reinterpret_cast<unsigned char *>(const_cast<char *>(s.c_str())),
          s.size());
      ASSERT_GE(i, 0) << "not found string '" << s
                      << "' on deserialization, j = " << j;
      unsigned int strlen;
      auto *se = deserialized->extract(i, &strlen);
      ASSERT_TRUE(se != nullptr) << i << " corresponding to " << s
                                 << " was not found on extract, j = " << j;
      std::string se_str(reinterpret_cast<char *>(se), strlen);
      ASSERT_EQ(se_str, s) << "Extracted str '" << se_str
                           << "' is different to '" << s << "', j = " << 4;
    }
  }
}

TEST(StringDictionaryHASHRPDACBlocksTests, can_create) {

  std::vector<std::string> data;

  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAC");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAE");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAF");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAI");
  data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJ");

  std::string false_data(
      "xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAX");

  std::sort(data.begin(), data.end());

  for (size_t j = 1; j <= 29; j++) {
    size_t total_size = 0;
    auto *plain_data = get_plain(data, total_size);

    auto *it = new IteratorDictStringPlain(
        reinterpret_cast<unsigned char *>(plain_data), total_size);

    StringDictionaryHASHRPDACBlocks sd(it, total_size, 25, 1 << j);

    for (size_t i = 1; i <= data.size(); i++) {
      uint extracted_sz = 0;
      auto *extracted = sd.extract(i, &extracted_sz);
      ASSERT_TRUE(extracted != nullptr)
          << "extracted " << i << " is null, j = " << j;
    }

    auto false_index = sd.locate(reinterpret_cast<unsigned char *>(
                                     const_cast<char *>(false_data.c_str())),
                                 false_data.size());
    ASSERT_EQ(false_index, NORESULT);
    for (auto &s : data) {
      auto i = sd.locate(
          reinterpret_cast<unsigned char *>(const_cast<char *>(s.c_str())),
          s.size());
      ASSERT_GT(i, 0) << "string '" << s << "' was not located, j = " << j;
      uint extracted_sz;
      auto *extracted = sd.extract(i, &extracted_sz);
      std::string extracted_str(reinterpret_cast<char *>(extracted),
                                extracted_sz);
      ASSERT_EQ(extracted_str, s)
          << "Extracted (i = " << i << ") " << extracted_str
          << " is different to " << s << ", j = " << j;
    }
  }
}

TEST(StringDictionaryHASHRPDACBlocksTests, parallel_build) {
  std::vector<std::string> data;

  for (int i = 0; i < 100000; i++) {
    char c = (char)((i % 20) + 'a');
    std::string extra(&c, 1);
    data.push_back("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" +
                   extra);
  }

  std::sort(data.begin(), data.end());
  {
    std::ofstream prueba("prueba.txt");
    for (auto &s : data) {
      prueba << s << "\n";
    }
  }
  size_t total_size = 0;
  auto *plain_data = get_plain(data, total_size);

  auto *it = new IteratorDictStringPlain(
      reinterpret_cast<unsigned char *>(plain_data), total_size);

  StringDictionaryHASHRPDACBlocks sd(
      it, total_size, 25, static_cast<unsigned long>(1UL << 20UL), 3);
  {
    std::ofstream ofs("parallel_build_test.sd",
                      std::ios::out | std::ios::binary);
    sd.save(ofs);
  }

  StringDictionary *deserialized;
  {
    std::ifstream ifs("parallel_build_test.sd",
                      std::ios::in | std::ios::binary);
    deserialized = StringDictionaryHASHRPDACBlocks::load(ifs);
  }

  std::string target = "xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa";
  ASSERT_GT(deserialized->locate(reinterpret_cast<unsigned char *>(
                                     const_cast<char *>(target.c_str())),
                                 target.size()),
            0)
      << "not found target str: " << target;
}

TEST(StringDictionaryHASHRPDACBlocksTests, extract_table) {
  std::vector<std::string> data;
  std::stringstream suffix_builder;
  std::string current_suf = "";
  for (int i = 0; i < 100000; i++) {
    char c = (char)((i % 20) + 'a');
    std::string extra(&c, 1);
    if (i % 20 == 0 && i / 20 > 0) {
      suffix_builder << extra;
      current_suf = suffix_builder.str();
    }
    extra = current_suf + extra;
    std::string result("xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" +
                       extra);
    data.push_back(result);
  }

  std::sort(data.begin(), data.end());
  {
    std::ofstream prueba("prueba.txt");
    for (auto &s : data) {
      prueba << s << "\n";
    }
  }
  size_t total_size = 0;
  auto *plain_data = get_plain(data, total_size);

  auto *it = new IteratorDictStringPlain(
      reinterpret_cast<unsigned char *>(plain_data), total_size);

  StringDictionaryHASHRPDACBlocks sd(
      it, total_size, 25, static_cast<unsigned long>(1UL << 20UL), 3);

  auto *table_it = sd.extractTable();
  while (table_it->hasNext()) {
    unsigned int sz;
    table_it->next(&sz);
    // std::cout << data << std::endl;
  }
  delete table_it;
}