#include "ConsoleInterpreter.hpp"
// STL
#include <memory>
#include <string>
#include <vector>
#include <sstream>
// fmt
#include <fmt/printf.h>
// internal
#include "TabId.h"
#include "MessageSearch.h"
#include "utilityString.h"
// #include "StorageAccess.h"

struct ICommand {
  virtual ~ICommand() = default;
  virtual std::string apply(const std::vector<std::string>& args) = 0;
};

struct FindCommand final : ICommand {
  ~FindCommand() override = default;
  std::string apply(const std::vector<std::string>& args) override {
    MessageSearch message({SearchMatch{utility::decodeFromUtf8(args[1])}}, NodeTypeSet::all());
    message.setSchedulerId(TabId::background());
    message.dispatch();
    return "DONE";
  }
};

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

ConsoleInterpreter::ConsoleInterpreter([[maybe_unused]] StorageAccess* pStorageAccess) noexcept {
  mCommands.emplace("find", std::make_shared<FindCommand>());
}

ConsoleInterpreter::~ConsoleInterpreter() = default;

std::string ConsoleInterpreter::process(const std::string& text) {
  if(auto tokens = split(text); tokens.size() > 1) {
    if(auto iterator = mCommands.find(tokens.front()); iterator != mCommands.end()) {
      return iterator->second->apply(tokens);
    }
  }

  return {};
}