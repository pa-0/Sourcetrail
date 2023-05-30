#include "FindCommand.hpp"
// STL
#include <fmt/format.h>
// internal
#include "FilePath.h"
#include "SourceLocationCollection.h"
#include "SourceLocationFile.h"
#include "StorageAccess.h"
#include "utilityString.h"

FindCommand::FindCommand() noexcept = default;

FindCommand::~FindCommand() noexcept = default;

std::string FindCommand::help() const noexcept {
  return "USAGE: command [options] text...";
}

std::string FindCommand::apply(StorageAccess& storageAccess,
                               const std::vector<std::string>& args) noexcept {
  if(args.empty()) {
    return {};
  }

  if(const auto pResult = storageAccess.getFullTextSearchLocations(
         utility::decodeFromUtf8(args.front()), false);
     pResult) {
    std::stringstream output;
    const auto sourceLocationCount = pResult->getSourceLocationCount();
    output << fmt::format("Found {} match.", sourceLocationCount);
    for(const auto& [filePath, pSourceLocationFile] : pResult->getSourceLocationFiles()) {
      for(const auto& pSourceLocation : pSourceLocationFile->getSourceLocations()) {
        output << fmt::format("\n- {}:{},{}",
                              utility::encodeToUtf8(filePath.fileName()),
                              pSourceLocation->getLineNumber(),
                              pSourceLocation->getStartLocation()->getColumnNumber());
      }
    }
    return output.str();
  }
  return "Not found.";
}