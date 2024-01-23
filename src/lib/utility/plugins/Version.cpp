#include "Version.h"
// fmt
#include <fmt/format.h>

namespace plugin {

bool Version::operator=(const Version& other) const {
  return minor == other.minor && major == other.major;
}

std::ostream& Version::operator<<(std::ostream& ostream) const {
  return ostream << fmt::format("{}.{}", major, minor);
}

}    // namespace plugin