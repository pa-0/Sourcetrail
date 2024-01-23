#include <iostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "ApplicationSettings.h"
#include "CommandLineParser.h"

struct CommandlineFix : testing::Test {
  void SetUp() override {
    appSettingsPath = ApplicationSettings::getInstance()->getFilePath();
    ApplicationSettings::getInstance()->load(FilePath(L"data/CommandlineTestSuite/settings.xml"));
  }
  void TearDown() override {
    ApplicationSettings::getInstance()->load(appSettingsPath);
  }
  FilePath appSettingsPath;
};

TEST_F(CommandlineFix, version) {
  std::vector<std::string> args({"--version", "help"});

  std::stringstream redStream;
  auto oldBuf = std::cout.rdbuf(redStream.rdbuf());

  commandline::CommandLineParser parser("2016.1");
  parser.preparse(args);
  parser.parse();

  std::cout.rdbuf(oldBuf);

  EXPECT_TRUE(redStream.str() == "Sourcetrail Version 2016.1\n");
}

TEST_F(CommandlineFix, commandConfigFilepathVector) {
  std::vector<std::string> args({"config", "-g", "/usr", "-g", "/usr/share/include", "-g", "/opt/test/include"});

  commandline::CommandLineParser parser("2");
  parser.preparse(args);
  parser.parse();

  std::vector<FilePath> paths = ApplicationSettings::getInstance()->getHeaderSearchPaths();
  EXPECT_TRUE(paths[0].wstr() == L"/usr");
  EXPECT_TRUE(paths[1].wstr() == L"/usr/share/include");
  EXPECT_TRUE(paths[2].wstr() == L"/opt/test/include");
}

TEST_F(CommandlineFix, commandConfigFilepathVectorCommaSeparated) {
  std::vector<std::string> args({"config", "--global-header-search-paths", "/usr,/usr/include,/include,/opt/include"});

  commandline::CommandLineParser parser("2");
  parser.preparse(args);
  parser.parse();

  std::vector<FilePath> paths = ApplicationSettings::getInstance()->getHeaderSearchPaths();
  EXPECT_TRUE(paths[0].wstr() == L"/usr");
  EXPECT_TRUE(paths[1].wstr() == L"/usr/include");
  EXPECT_TRUE(paths[2].wstr() == L"/include");
  EXPECT_TRUE(paths[3].wstr() == L"/opt/include");
}

TEST_F(CommandlineFix, commandConfigBoolOptions) {
  std::vector<std::string> args({"config", "--use-processes", "false"});

  commandline::CommandLineParser parser("2");
  parser.preparse(args);
  parser.parse();

  bool processes = ApplicationSettings::getInstance()->getMultiProcessIndexingEnabled();
  EXPECT_TRUE(processes == false);

  std::vector<std::string> args1({"config", "--use-processes", "true"});

  parser.preparse(args1);
  parser.parse();

  processes = ApplicationSettings::getInstance()->getMultiProcessIndexingEnabled();
  EXPECT_TRUE(processes == true);
}
