
#include <cppUtils.h>
#include <libcdsBasics.h>

namespace cds_utils {

uint transform(const string &s) {
  stringstream ss;
  ss << s;
  uint ret;
  ss >> ret;
  return ret;
}

} // namespace cds_utils
