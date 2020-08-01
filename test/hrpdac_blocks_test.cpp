#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>
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

  std::sort(data.begin(), data.end());

  size_t total_size = 0;
  auto *plain_data = get_plain(data, total_size);

  auto *it = new IteratorDictStringPlain(
      reinterpret_cast<unsigned char *>(plain_data), total_size);

  StringDictionaryHASHRPDACBlocks sd(it, total_size, 25);
  for (size_t i = 1; i <= data.size(); i++) {
    uint extracted_sz = 0;
    auto *extracted = sd.extract(i, &extracted_sz);
    ASSERT_TRUE(extracted) << "extracted " << i << " is null";
  }

  for (auto &s : data) {
    auto i = sd.locate(
        reinterpret_cast<unsigned char *>(const_cast<char *>(s.c_str())),
        s.size());
    ASSERT_GT(i, 0) << "string '" << s << "' was not located";
    uint extracted_sz;
    auto *extracted = sd.extract(i, &extracted_sz);
    std::string extracted_str(reinterpret_cast<char *>(extracted),
                              extracted_sz);
    ASSERT_EQ(extracted_str, s) << "Extracted (i = " << i << ") "
                                << extracted_str << " is different to " << s;
  }
}