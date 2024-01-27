#include <algorithm>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CommandlineCommand.h"
#include "utilities/CollectOutStream.hpp"

using namespace testing;

struct MockedCommandlineCommand : public commandline::CommandlineCommand {
  using Args = std::vector<std::string>;
  using ReturnStatus = CommandlineCommand::ReturnStatus;

  MockedCommandlineCommand(std::string name, std::string description)
      : commandline::CommandlineCommand(std::move(name), std::move(description), nullptr) {}
  MOCK_METHOD(void, setup, (), (override));
  MOCK_METHOD(ReturnStatus, parse, (Args&), (override));
  MOCK_METHOD(bool, hasHelp, (), (const, override));

  using commandline::CommandlineCommand::m_positional;
};

// NOLINTNEXTLINE
TEST(CommandlineCommand, goodCase) {
  const std::string name = "name";
  const std::string desc = "desc";
  MockedCommandlineCommand mockedCommandLine(name, desc);


  EXPECT_THAT(mockedCommandLine.name(), StrEq(name));
  EXPECT_THAT(mockedCommandLine.description(), StrEq(desc));
  EXPECT_THAT(mockedCommandLine.description(), StrEq(desc));

  CollectOutStream collectStreamBefore(std::cout);
  mockedCommandLine.printHelp();
  collectStreamBefore.close();
  constexpr std::string_view HelpMessageBefore = "Usage:\n\n  Sourcetrail name [option...]\n\ndesc\n\n\n";
  EXPECT_THAT(collectStreamBefore.str(), StrEq(HelpMessageBefore));

  mockedCommandLine.m_positional.add("pos", 1);

  CollectOutStream collectStream(std::cout);
  mockedCommandLine.printHelp();
  collectStream.close();
  constexpr std::string_view HelpMessage =
      "Usage:\n\n  Sourcetrail name [option...]\n\ndesc\n\n\nPositional Arguments: \n  1: pos\n";
  EXPECT_THAT(collectStream.str(), StrEq(HelpMessage));
}