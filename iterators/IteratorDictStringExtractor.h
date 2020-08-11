#ifndef _ITERATORDICTSTRING_EXTRACTOR_H
#define _ITERATORDICTSTRING_EXTRACTOR_H

#include "IteratorDictString.h"
#include "StringDictionary.h"

class IteratorDictStringExtractor : public IteratorDictString {
  StringDictionary *sd;
  unsigned long current;

public:
  IteratorDictStringHRPDACBlocks(StringDictionary *sd) : sd(sd), current(1) {}

  bool hasNext() { return current <= sd->numElements(); }

  unsigned char *next(uint *str_length) {
    return sd->extract(current++, str_length);
  }
};

#endif /* _ITERATORDICTSTRING_EXTRACTOR_H */