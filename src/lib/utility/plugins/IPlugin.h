#pragma once
// STL
#include <string>
// internal
#include "Version.h"

namespace plugin {

struct IPlugin {
  virtual ~IPlugin() noexcept;

  virtual std::string name() const = 0;

  virtual Version version() const = 0;
};

} // namespace plugin