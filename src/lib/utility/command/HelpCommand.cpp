// internal
#include "HelpCommand.hpp"

HelpCommand::HelpCommand() noexcept = default;

HelpCommand::~HelpCommand() noexcept = default;

std::string HelpCommand::apply(StorageAccess& /*pStorageAccess*/, const std::vector<std::string>&  /*args*/) { return {}; }