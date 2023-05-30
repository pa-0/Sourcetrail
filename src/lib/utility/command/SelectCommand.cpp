// internal
#include "SelectCommand.hpp"

SelectCommand::SelectCommand() noexcept = default;

SelectCommand::~SelectCommand() noexcept = default;

std::string SelectCommand::apply(StorageAccess& /*pStorageAccess*/, const std::vector<std::string>&  /*args*/) { return {}; }