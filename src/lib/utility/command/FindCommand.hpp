// internal
#include "ICommand.hpp"

struct FindCommand final : ICommand {
  FindCommand() noexcept;

  FindCommand(const FindCommand&) = delete;
  FindCommand& operator=(const FindCommand&) = delete;
  FindCommand(FindCommand&&) = delete;
  FindCommand& operator=(FindCommand&&) = delete;

  ~FindCommand() noexcept override;

  [[nodiscard]] std::string help() const noexcept override;

  std::string apply(StorageAccess& pStorageAccess, const std::vector<std::string>& args) noexcept override;
};