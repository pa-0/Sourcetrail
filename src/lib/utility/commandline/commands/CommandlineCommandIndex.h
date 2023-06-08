#pragma once

#include "CommandlineCommand.h"

namespace commandline {

class CommandlineCommandIndex : public CommandlineCommand {
public:
  CommandlineCommandIndex(CommandLineParser* parser);

  ~CommandlineCommandIndex() override;

  void setup() override;

  ReturnStatus parse(std::vector<std::string>& args) override;

  [[nodiscard]] bool hasHelp() const override {
    return true;
  }
};

}    // namespace commandline
