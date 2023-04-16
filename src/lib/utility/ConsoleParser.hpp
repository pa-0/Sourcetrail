#pragma once
// STL
#include <memory>
#include <string>
#include <unordered_map>

struct ICommand;
class StorageAccess;

class ConsoleParser final {
public:
  explicit ConsoleParser(StorageAccess* pStorageAccess) noexcept;

  std::string process(const std::string& text);

private:
  std::unordered_map<std::string, std::shared_ptr<ICommand>> mCommands;
};