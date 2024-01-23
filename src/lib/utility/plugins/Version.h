// STL
#include <cstdint>
#include <ostream>
#include <string>

namespace plugin {

struct Version {
  uint32_t minor = 0;
  uint32_t major = 0;

  bool operator=(const Version& other) const;

  std::ostream& operator<<(std::ostream& ostream) const;
};

}    // namespace plugin