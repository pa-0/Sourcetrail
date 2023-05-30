// internal
#include "ICommand.hpp"

struct ShowCommand final : ICommand {
  ShowCommand() noexcept;

  ShowCommand(const ShowCommand&) = delete;
  ShowCommand& operator=(const ShowCommand&) = delete;
  ShowCommand(ShowCommand&&) = delete;
  ShowCommand& operator=(ShowCommand&&) = delete;

  ~ShowCommand() noexcept override;

  std::string apply(StorageAccess& pStorageAccess, const std::vector<std::string>& args) override;
};