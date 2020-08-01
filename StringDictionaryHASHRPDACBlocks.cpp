#include "StringDictionaryHASHRPDACBlocks.h"

#include <algorithm>
#include <cppUtils.h>
#include <memory>
#include <string_view>

StringDictionaryHASHRPDACBlocks::StringDictionaryHASHRPDACBlocks(
    IteratorDictStringPlain *it, unsigned long len, int overhead)
    : StringDictionaryHASHRPDACBlocks(it, len, overhead, DEFAULT_CUT_SIZE) {}

StringDictionaryHASHRPDACBlocks::StringDictionaryHASHRPDACBlocks(
    IteratorDictStringPlain *it, unsigned long, int overhead,
    unsigned long cut_size)
    : cut_size(cut_size) {

  unsigned long acc_size = 0;
  unsigned long offset = 0;
  unsigned long strings_qty = 0;

  bool sample_next = true;

  while (it->hasNext()) {
    unsigned int next_string_length;
    auto *next_string = it->next(&next_string_length);

    if (sample_next) {
      cut_samples.emplace_back(reinterpret_cast<char *>(next_string),
                               next_string_length);
      starting_indexes.push_back(strings_qty);
      sample_next = false;
    }

    acc_size += next_string_length + 1;
    strings_qty++;

    if (!it->hasNext() || acc_size + next_string_length > cut_size) {
      auto *sub_it =
          new IteratorDictStringPlain(it->getPlainText() + offset, acc_size);
      sub_it->keep_buffer();

      parts.push_back(new StringDictionaryHASHRPDAC(sub_it, 0, overhead));

      offset += acc_size;

      acc_size = 0;
      sample_next = true;
    }
  }
  delete it;
}

unsigned long StringDictionaryHASHRPDACBlocks::locate(uchar *str,
                                                      uint str_length) {
  auto sview = std::string_view(reinterpret_cast<char *>(str), str_length);
  auto ubound_it =
      std::lower_bound(cut_samples.begin(), cut_samples.end(), sview);
  unsigned long result_pos;
  if (ubound_it == cut_samples.end())
    result_pos = parts.size() - 1;
  else
    result_pos = ubound_it - cut_samples.begin();

  return parts[result_pos]->locate(str, str_length) +
         starting_indexes[result_pos];
}

uchar *StringDictionaryHASHRPDACBlocks::extract(size_t id, uint *str_length) {
  auto ubound_it = std::lower_bound(starting_indexes.begin(),
                                    starting_indexes.end(), id - 1);
  unsigned long result_pos;
  if (ubound_it == starting_indexes.end())
    result_pos = parts.size() - 1;
  else
    result_pos = ubound_it - starting_indexes.begin();
  auto sindex = starting_indexes[result_pos];
  auto eindex = id - sindex;

  return parts[result_pos]->extract(eindex, str_length);
}

IteratorDictString *StringDictionaryHASHRPDACBlocks::extractTable() {
  std::vector<uchar *> tabledec(starting_indexes[starting_indexes.size() - 1]);
  uint strLen;
  for (unsigned long partIdx = 1; partIdx < starting_indexes.size();
       partIdx++) {
    for (unsigned long i = 1;
         i <= starting_indexes[partIdx] - starting_indexes[partIdx - 1] + 1;
         i++)
      tabledec[starting_indexes[partIdx - 1] + i - 1] =
          parts[partIdx - 1]->extract(i, &strLen);
  }

  return new IteratorDictStringVector(&tabledec, elements);
}

size_t StringDictionaryHASHRPDACBlocks::getSize() {
  size_t result = 0;
  for (auto *part : parts) {
    result += part->getSize();
  }
  result += parts.size() * sizeof(StringDictionaryHASHRPDAC *);
  result += starting_indexes.size() * sizeof(unsigned long);
  for (auto &s : cut_samples) {
    result += s.size();
  }
  return result;
}

void StringDictionaryHASHRPDACBlocks::save(std::ofstream &out) {
  saveValue<uint64_t>(out, cut_size);
  saveValue<uint32_t>(out, parts.size());

  for (uint i = 0; i < cut_samples.size(); i++) {
    auto &s = cut_samples[i];
    saveValue<uint32_t>(out, s.size());
    saveValue<char>(out, s.c_str(), s.size());
  }

  for (uint i = 1; i < starting_indexes.size(); i++) {
    saveValue<uint64_t>(out, starting_indexes[i]);
  }

  for (auto *part : parts) {
    part->save(out);
  }
}

StringDictionary *StringDictionaryHASHRPDACBlocks::load(std::ifstream &in,
                                                        uint technique) {
  auto _cut_size = loadValue<uint64_t>(in);
  auto parts_sz = loadValue<uint32_t>(in);
  std::vector<std::string> _cut_samples;
  std::vector<unsigned long> _starting_indexes;
  std::vector<StringDictionary *> _parts;
  for (uint i = 0; i < parts_sz; i++) {
    auto cut_sample_sz = loadValue<uint32_t>(in);
    auto *cut_sample_buf = loadValue<char>(in, cut_sample_sz);
    _cut_samples.emplace_back(cut_sample_buf, cut_sample_sz);
  }
  for (uint i = 0; i < parts_sz; i++) {
    loadValue<uint64_t>(in);
  }

  for (uint i = 0; i < parts_sz; i++) {
    _parts.push_back(StringDictionaryHASHRPDAC::load(in, technique));
  }

  StringDictionary *result = new StringDictionaryHASHRPDACBlocks(
      _cut_size, std::move(_parts), std::move(_cut_samples),
      std::move(_starting_indexes));

  return result;
}

StringDictionaryHASHRPDACBlocks::StringDictionaryHASHRPDACBlocks(
    unsigned long cut_size, std::vector<StringDictionary *> &&parts,
    std::vector<std::string> &&cut_samples,
    std::vector<unsigned long> &&starting_indexes)
    : cut_size(cut_size), parts(std::move(parts)),
      cut_samples(std::move(cut_samples)),
      starting_indexes(std::move(starting_indexes)) {}

StringDictionaryHASHRPDACBlocks::~StringDictionaryHASHRPDACBlocks() {
  for (auto *sd : parts) {
    delete sd;
  }
}

IteratorDictID *StringDictionaryHASHRPDACBlocks::locatePrefix(uchar *, uint) {
  std::cout << "This dictionary does not provide prefix location" << std::endl;
  return NULL;
}

IteratorDictID *StringDictionaryHASHRPDACBlocks::locateSubstr(uchar *, uint) {
  std::cout << "This dictionary does not provide substring location"
            << std::endl;
  return NULL;
}

uint StringDictionaryHASHRPDACBlocks::locateRank(uint) {
  std::cout << "This dictionary does not provide rank location" << std::endl;
  return 0;
}

IteratorDictString *StringDictionaryHASHRPDACBlocks::extractPrefix(uchar *,
                                                                   uint) {
  std::cout << "This dictionary does not provide prefix extraction"
            << std::endl;
  return NULL;
}

IteratorDictString *StringDictionaryHASHRPDACBlocks::extractSubstr(uchar *,
                                                                   uint) {
  std::cout << "This dictionary does not provide substring extraction"
            << std::endl;
  return NULL;
}

uchar *StringDictionaryHASHRPDACBlocks::extractRank(uint, uint *) {
  std::cout << "This dictionary does not provide rank extraction" << std::endl;
  return NULL;
}
