// internal
#include "ICommand.hpp"

struct SelectCommand final : ICommand {
  SelectCommand() noexcept;

  SelectCommand(const SelectCommand&) = delete;
  SelectCommand& operator=(const SelectCommand&) = delete;
  SelectCommand(SelectCommand&&) = delete;
  SelectCommand& operator=(SelectCommand&&) = delete;

  ~SelectCommand() noexcept override;

  std::string apply(StorageAccess& pStorageAccess, const std::vector<std::string>& args) override;
};