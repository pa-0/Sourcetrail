#include "ConsoleInterpreter.hpp"
// STL
#include <memory>
#include <string>
#include <vector>
#include <sstream>
// fmt
#include <fmt/printf.h>
// internal
#include "command/ICommand.hpp"
#include "TabId.h"
#include "MessageSearch.h"
#include "utilityString.h"
// #include "StorageAccess.h"

namespace {
  std::vector<std::string> split(const std::string& text) {
    std::vector<std::string> output;
    std::stringstream iStream(text);
    for(std::string token; std::getline(iStream, token, ' ');) {
      output.emplace_back(token);
    }
    return output;
  }
}

ConsoleInterpreter::ConsoleInterpreter(StorageAccess* pStorageAccess) noexcept
  : m_pStorageAccess{pStorageAccess} {
  // mCommands.emplace("find",   std::make_shared<FindCommand>());
  // mCommands.emplace("help",   std::make_shared<HelpCommand>());
  // mCommands.emplace("select", std::make_shared<SelectCommand>());
  // mCommands.emplace("show",   std::make_shared<ShowCommand>());
}

ConsoleInterpreter::~ConsoleInterpreter() = default;

std::string ConsoleInterpreter::process(const std::string& text) {
  if(auto tokens = split(text); tokens.size() > 1) {
    if(auto iterator = mCommands.find(tokens.front()); iterator != mCommands.end()) {
      return iterator->second->apply(*m_pStorageAccess, tokens);
    }
  }

  return {};
}