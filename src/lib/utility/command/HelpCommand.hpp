// internal
#include "ICommand.hpp"

struct HelpCommand final : ICommand {
  HelpCommand() noexcept;

  HelpCommand(const HelpCommand&) = delete;
  HelpCommand& operator=(const HelpCommand&) = delete;
  HelpCommand(HelpCommand&&) = delete;
  HelpCommand& operator=(HelpCommand&&) = delete;

  ~HelpCommand() noexcept override;

  std::string apply(StorageAccess& pStorageAccess, const std::vector<std::string>& args) override;
};