// STL
#include <string>
#include <vector>

class StorageAccess;

struct ICommand {
  virtual ~ICommand() noexcept = default;
  [[nodiscard]] virtual std::string help() const noexcept = 0;
  virtual std::string apply(StorageAccess& storageAccess, const std::vector<std::string>& args) noexcept = 0;
};