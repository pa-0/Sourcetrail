#pragma once
// STL
#include <memory>
#include <string>
#include <unordered_map>

struct ICommand;
class StorageAccess;

struct IConsoleInterpreter {
  virtual ~IConsoleInterpreter() = default;
  virtual std::string process(const std::string& text) = 0;
};

class ConsoleInterpreter final : public IConsoleInterpreter {
public:
  explicit ConsoleInterpreter(StorageAccess* pStorageAccess) noexcept;

  ConsoleInterpreter(const ConsoleInterpreter&) = delete;
  ConsoleInterpreter& operator=(const ConsoleInterpreter&) = delete;
  ConsoleInterpreter(ConsoleInterpreter&&) = delete;
  ConsoleInterpreter& operator=(ConsoleInterpreter&&) = delete;

  ~ConsoleInterpreter() override;

  std::string process(const std::string& text) override;

private:
  StorageAccess* m_pStorageAccess;
  std::unordered_map<std::string, std::shared_ptr<ICommand>> mCommands;
};