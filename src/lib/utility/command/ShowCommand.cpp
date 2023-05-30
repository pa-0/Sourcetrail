// internal
#include "ShowCommand.hpp"

ShowCommand::ShowCommand() noexcept = default;

ShowCommand::~ShowCommand() noexcept = default;

std::string ShowCommand::apply(StorageAccess& /*pStorageAccess*/, const std::vector<std::string>&  /*args*/) { return {}; }