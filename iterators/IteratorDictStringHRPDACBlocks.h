#ifndef _ITERATORDICTSTRING_HRPDACBLOCKS_H
#define _ITERATORDICTSTRING_HRPDACBLOCKS_H

#include "IteratorDictString.h"
#include "StringDictionary.h"
#include <vector>

class IteratorDictStringHRPDACBlocks : public IteratorDictString {
  const std::vector<StringDictionary *> &parts;
  const std::vector<unsigned long> &starting_indexes;
  unsigned long strings_qty;

  unsigned long current;
  unsigned long partIdx;

  unsigned long to_index() {
    return partIdx < starting_indexes.size() - 1 ? starting_indexes[partIdx + 1]
                                                 : strings_qty;
  }

public:
  explicit IteratorDictStringHRPDACBlocks(
      const std::vector<StringDictionary *> &parts,
      const std::vector<unsigned long> &starting_indexes,
      unsigned long strings_qty)
      : parts(parts), starting_indexes(starting_indexes),
        strings_qty(strings_qty), current(1), partIdx(0) {}

  bool hasNext() {
    return partIdx < parts.size() &&
           current <= to_index() - starting_indexes[partIdx];
  }

  unsigned char *next(uint *str_length) {
    auto *result = parts[partIdx]->extract(current++, str_length);

    if (current > to_index() - starting_indexes[partIdx]) {
      partIdx++;
      current = 1;
    }

    return result;
  }
};

#endif /* _ITERATORDICTSTRING_HRPDACBLOCKS_H */
