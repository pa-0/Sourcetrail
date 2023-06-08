// STL
#include <memory>
// gtest
#include <gmock/gmock.h>
#include <gtest/gtest.h>
// internal
#include "SourceLocation.h"
#include "SourceLocationCollection.h"
#include "command/FindCommand.hpp"
#include "mocks/MockedStorageAccess.hpp"

using namespace testing;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory,cert-err58-cpp)
TEST(HelpFindCommand, goodcase) {
  FindCommand findCommand;

  constexpr std::string_view OutputText = "USAGE: command [options] text...";
  EXPECT_THAT(findCommand.help(), StrEq(OutputText));
}

class FindCommandFix : public Test {
public:
  void SetUp() override {}

  void TearDown() override {}

  void setupSourceLocation() {
    auto pLocationCollection = std::make_shared<SourceLocationCollection>();
    const FilePath filePath {L"main.cpp"};
    pLocationCollection->addSourceLocation(LOCATION_FULLTEXT_SEARCH, 1, {1}, filePath, 1, 1, 4, 1);

    std::wstring searchTerm = L"main";
    EXPECT_CALL(mMockedStroageAccess, getFullTextSearchLocations(searchTerm, false))
        .WillOnce(Return(pLocationCollection));
  }

  StrictMock<MockedStorageAccess> mMockedStroageAccess;
};
  
// NOLINTNEXTLINE(cert-err58-cpp,cppcoreguidelines-owning-memory,cppcoreguidelines-avoid-non-const-global-variables)
TEST_F(FindCommandFix, emptyArgs) {
  FindCommand findCommand;
  EXPECT_THAT(findCommand.apply(mMockedStroageAccess, {}), IsEmpty());
}

// NOLINTNEXTLINE(cert-err58-cpp,cppcoreguidelines-owning-memory,cppcoreguidelines-avoid-non-const-global-variables)
TEST_F(FindCommandFix, findText) {
  setupSourceLocation();

  FindCommand findCommand;
  constexpr std::string_view OutputText = "Found 1 match.\n- main.cpp:1,1\n- main.cpp:4,1";
  EXPECT_THAT(findCommand.apply(mMockedStroageAccess, {"main"}), StrEq(OutputText));
}

// NOLINTNEXTLINE(cert-err58-cpp,cppcoreguidelines-owning-memory,cppcoreguidelines-avoid-non-const-global-variables)
TEST_F(FindCommandFix, textNotFound) {
  FindCommand findCommand;

  std::wstring searchTerm = L"main";

  EXPECT_CALL(mMockedStroageAccess, getFullTextSearchLocations(searchTerm, false))
      .WillOnce(Return(nullptr));

  EXPECT_THAT(findCommand.apply(mMockedStroageAccess, {"main"}), StrEq("Not found."));
}