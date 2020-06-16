
#include <cppUtils.h>
#include <libcdsBasics.h>

namespace cds_utils {

uint transform(const std::string &s) {
  std::stringstream ss;
  ss << s;
  uint ret;
  ss >> ret;
  return ret;
}

} // namespace cds_utils
