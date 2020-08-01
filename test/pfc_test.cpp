#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <StringDictionaryPFC.h>

class FileSetIteratorDictString : public IteratorDictString {
  std::istream &ifs;
  std::string current_line;

public:
  explicit FileSetIteratorDictString(std::istream &ifs) : ifs(ifs) {}

  bool hasNext() override { return ifs && ifs.peek() != EOF; }

  unsigned char *next(uint *str_length) override {
    std::getline(ifs, current_line);
    auto *uchar_val = new unsigned char[current_line.size() + 1];
    *str_length = current_line.size();
    memcpy(uchar_val, current_line.data(), current_line.size());
    uchar_val[current_line.size()] = '\0';
    return uchar_val;
  }
};

const std::string TEST_STRS_1[] = {
    "AAAAAAAAAB", "AAAAAAAAAC", "AAAAAAAAAD", "AAAAAAAAAE", "AAAAAAAAAF",
    "AAAAAAAAAG", "AAAAAAAAAH", "AAAAAAAAAI", "AAAAAAAAAJ", "AAAAAAAAAK",
    "AAAAAAAAAL", "AAAAAAAAAM", "AAAAAAAAAN",
};

constexpr size_t TEST_STRS_1_SZ = sizeof(TEST_STRS_1) / sizeof(TEST_STRS_1[0]);

std::string test_str_1() {
  std::stringstream ss;
  for (size_t i = 0; i < TEST_STRS_1_SZ; i++) {
    ss << TEST_STRS_1[i] << "\n";
  }

  return ss.str();
}

struct result_gen {
  std::string s;
  std::vector<std::string> s_v;
};

result_gen generate_i_strings(int i) {
  std::stringstream ss;
  result_gen r;
  for (int j = 0; j < i; j++) {
    r.s_v.push_back(std::to_string(j));
  }
  std::sort(r.s_v.begin(), r.s_v.end());
  for (auto &s : r.s_v) {
    ss << s << "\n";
  }
  r.s = ss.str();
  return r;
}

TEST(pfc_tests, test_extract) {
  auto s = test_str_1();

  std::istringstream iss(s);

  auto *it = new FileSetIteratorDictString(iss);
  StringDictionaryPFC sd(it, 4096);

  for (size_t i = 1; i <= sd.numElements(); i++) {
    unsigned int str_len;
    unsigned char *s_ext = sd.extract(i, &str_len);
    std::string_view sv(reinterpret_cast<char *>(s_ext), str_len);

    ASSERT_EQ(sv, TEST_STRS_1[i - 1]);

    delete[] s_ext;
  }
}

TEST(pfc_tests, test_locate) {
  auto s = test_str_1();

  std::istringstream iss(s);

  auto *it = new FileSetIteratorDictString(iss);
  StringDictionaryPFC sd(it, 4096);
  for (size_t i = 1; i <= sd.numElements(); i++) {
    unsigned int str_len;
    unsigned char *s_ext = sd.extract(i, &str_len);
    unsigned long id = sd.locate(s_ext, str_len);
    ASSERT_EQ(id, i);
    std::string_view sv(reinterpret_cast<char *>(s_ext), str_len);
    ASSERT_EQ(sv, TEST_STRS_1[i - 1]);

    delete[] s_ext;
  }
}

TEST(pfc_tests, test_extract_2) {
  auto r = generate_i_strings(1000000);

  std::istringstream iss(r.s);

  auto *it = new FileSetIteratorDictString(iss);
  StringDictionaryPFC sd(it, 4096);

  std::cout << "Built... now checking" << std::endl;
  for (size_t i = 1; i <= sd.numElements(); i++) {
    unsigned int str_len;
    unsigned char *s_ext = sd.extract(i, &str_len);
    std::string_view sv(reinterpret_cast<char *>(s_ext), str_len);

    ASSERT_EQ(sv, r.s_v[i - 1]);

    delete[] s_ext;
  }
}

TEST(pfc_tests, test_locate_2) {
  auto r = generate_i_strings(1000000);

  std::istringstream iss(r.s);

  auto *it = new FileSetIteratorDictString(iss);
  StringDictionaryPFC sd(it, 4096);
  std::cout << "Built... now checking" << std::endl;
  for (size_t i = 1; i <= sd.numElements(); i++) {
    unsigned int str_len;
    unsigned char *s_ext = sd.extract(i, &str_len);
    unsigned long id = sd.locate(s_ext, str_len);
    ASSERT_EQ(id, i) << "Failed at i = " << i;
    delete[] s_ext;
  }
}