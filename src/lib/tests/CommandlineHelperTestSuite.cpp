#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CommandlineHelper.h"
#include "FilePath.h"
#include "utilities/FileHandler.hpp"

using namespace testing;

namespace po = boost::program_options;

// NOLINTNEXTLINE
TEST(CommandLineHelper, emptyVaraiblesMapParseConfigFile) {
  po::variables_map variablesMap;
  po::options_description options;
  commandline::parseConfigFile(variablesMap, options);
  EXPECT_TRUE(variablesMap.empty());
}

// NOLINTNEXTLINE
TEST(CommandLineHelper, missingParseConfigFile) {
  po::options_description desc("Allowed options");
  // clang-format off
  desc.add_options()
    ("config-file", po::value<std::string>(), "");
  // clang-format on

  int argc = 3;
  char* argv[] = {"", "--config-file", "something"};

  po::variables_map variablesMap;
  po::store(po::parse_command_line(argc, argv, desc), variablesMap);
  po::notify(variablesMap);

  po::options_description options;
  commandline::parseConfigFile(variablesMap, options);
  EXPECT_EQ(1, variablesMap.count("config-file"));
  EXPECT_THAT(variablesMap["config-file"].as<std::string>(), StrEq("something"));
}

// NOLINTNEXTLINE
TEST(CommandLineHelper, goodCaseParseConfigFile) {
  constexpr auto ConfigFile = "/tmp/config";
  auto fileHandler = FileHandler::createEmptyFile(ConfigFile);

  po::options_description desc("Allowed options");
  // clang-format off
  desc.add_options()
    ("config-file", po::value<std::string>(), "");
  // clang-format on

  int argc = 3;
  char* argv[] = {"", "--config-file", "/tmp/config"};

  po::variables_map variablesMap;
  po::store(po::parse_command_line(argc, argv, desc), variablesMap);
  po::notify(variablesMap);

  po::options_description options;
  commandline::parseConfigFile(variablesMap, options);
  EXPECT_EQ(1, variablesMap.size());
  EXPECT_EQ(1, variablesMap.count("config-file"));
  EXPECT_THAT(variablesMap["config-file"].as<std::string>(), StrEq("/tmp/config"));
}

// NOLINTNEXTLINE
TEST(CommandLineHelper, goodCaseExtractPaths) {
  EXPECT_THAT(commandline::extractPaths({}), IsEmpty());

  const auto inputs0 = commandline::extractPaths({"/usr/include/,/usr/share/Catch2"});
  std::vector<std::string> inputsString0;
  std::transform(std::cbegin(inputs0), std::cend(inputs0), std::back_inserter(inputsString0), [](const FilePath& value) {
    return value.str();
  });
  EXPECT_THAT(inputsString0, Eq(std::vector<std::string> {"/usr/include/", "/usr/share/Catch2"}));

  const auto inputs1 = commandline::extractPaths({"/usr/include/", "/usr/share/Catch2"});
  std::vector<std::string> inputsString1;
  std::transform(std::cbegin(inputs1), std::cend(inputs1), std::back_inserter(inputsString1), [](const FilePath& value) {
    return value.str();
  });
  EXPECT_THAT(inputsString1, Eq(std::vector<std::string> {"/usr/include/", "/usr/share/Catch2"}));
}
