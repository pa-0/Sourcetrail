#include <array>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CommandLineParser.h"

using namespace ::testing;
using namespace std::string_literals;

struct CollectOutStream final {
  explicit CollectOutStream(std::ostream& ostream) : mStream {ostream}, mOldBuffer {mStream.rdbuf()} {
    mStream.rdbuf(mBufferStream.rdbuf());
  }

  ~CollectOutStream() {
    close();
  }

  void close() {
    if(mOldBuffer != nullptr) {
      mStream.rdbuf(mOldBuffer);
      mOldBuffer = nullptr;
    }
  }

  std::string str() const {
    return mBufferStream.str();
  }

  CollectOutStream(const CollectOutStream&) = delete;
  CollectOutStream& operator=(const CollectOutStream&) = delete;
  CollectOutStream(CollectOutStream&&) = delete;
  CollectOutStream& operator=(CollectOutStream&&) = delete;

private:
  std::ostream& mStream;
  std::streambuf* mOldBuffer;
  std::stringstream mBufferStream;
};

// NOLINTNEXTLINE
TEST(CommandLineParser, initStateEmptyArgs) {
  std::vector<std::string> args;
  commandline::CommandLineParser parser({});
  parser.preparse(args);
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_FALSE(parser.exitApplication());
  EXPECT_FALSE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

// NOLINTNEXTLINE
TEST(CommandLineParserConfig, helpOption) {
  std::vector<std::string> args {"--help"};
  commandline::CommandLineParser parser({});
  {
    constexpr std::string_view HelpString =
        "Usage:\n  Sourcetrail [command] [option...] [positional arguments]\n\nCommands:\n  config                 Change "
        "preferences relevant to project indexing.*\n  index                  Index a certain project.*\n\n  * has its own "
        "--help\n\nOptions:\n  -h [ --help ]          Print this help message\n  -v [ --version ]       Version of Sourcetrail\n "
        " --project-file arg     Open Sourcetrail with this project (.srctrlprj)\n\nPositional Arguments: \n  1: project-file\n";
    CollectOutStream collectCout(std::cout);
    parser.preparse(args);
    collectCout.close();
    EXPECT_THAT(collectCout.str(), StrEq(HelpString));
  }
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_TRUE(parser.exitApplication());
  EXPECT_FALSE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

// NOLINTNEXTLINE
TEST(CommandLineParserConfig, versionOption) {
  constexpr std::string_view VersionString = "Sourcetrail Version 2023.6.8\n";
  std::vector<std::string> args {"--version"};
  commandline::CommandLineParser parser("2023.6.8");
  {
    CollectOutStream collectCout(std::cout);
    parser.preparse(args);
    collectCout.close();
    EXPECT_THAT(collectCout.str(), VersionString);
  }
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_TRUE(parser.exitApplication());
  EXPECT_FALSE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

// NOLINTNEXTLINE
TEST(CommandLineParserConfig, projectFileOptionIsEmpty) {
  constexpr std::string_view MissingProjectString =
      "ERROR: the required argument for option '--project-file' is missing\n\n\nOptions:\n  -h [ --help ]          Print this "
      "help message\n  -v [ --version ]       Version of Sourcetrail\n  --project-file arg     Open Sourcetrail with this "
      "project (.srctrlprj)\n\n";
  std::vector<std::string> args {"--project-file"};
  commandline::CommandLineParser parser({});
  {
    CollectOutStream collectCout(std::cout);
    CollectOutStream collectCerr(std::cerr);
    parser.preparse(args);
    collectCout.close();
    collectCerr.close();
    EXPECT_THAT(collectCout.str(), IsEmpty());
    EXPECT_THAT(collectCerr.str(), StrEq(MissingProjectString));
  }
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_FALSE(parser.exitApplication());
  EXPECT_FALSE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

// NOLINTNEXTLINE
TEST(CommandLineParserConfig, projectFileOption) {
  std::vector<std::string> args {"--project-file", "not-exists"};
  commandline::CommandLineParser parser({});
  parser.preparse(args);
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_FALSE(parser.exitApplication());
  EXPECT_TRUE(parser.hasError());
  const std::wstring GeneratedErrorMessage = L"Provided Projectfile is not valid:\n* Provided Projectfile('not-exists')  does not exist";
  EXPECT_THAT(parser.getError(), StrEq(GeneratedErrorMessage));
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

/*
// NOLINTNEXTLINE
TEST(CommandLineParserConfig, emptyProjectFile) {
  std::vector<std::string> args{"not-exists-project"};
  commandline::CommandLineParser parser({});
  parser.preparse(args);
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_FALSE(parser.exitApplication());
  EXPECT_TRUE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

// NOLINTNEXTLINE
TEST(CommandLineParserConfig, initStateNoRegisitedCommands) {
  std::vector<std::string> args{"somthing"};
  commandline::CommandLineParser parser({});
  parser.preparse(args);
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_FALSE(parser.exitApplication());
  EXPECT_TRUE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}

// NOLINTNEXTLINE
TEST(CommandLineParserConfig, initStateRegisitedCommands) {
  std::vector<std::string> args{"config --help"};
  commandline::CommandLineParser parser({});
  parser.preparse(args);
  EXPECT_FALSE(parser.runWithoutGUI());
  EXPECT_FALSE(parser.exitApplication());
  EXPECT_TRUE(parser.hasError());
  EXPECT_EQ(RefreshMode::REFRESH_NONE, parser.getShallowIndexingRequested());
  EXPECT_FALSE(parser.getShallowIndexingRequested());
}
*/
