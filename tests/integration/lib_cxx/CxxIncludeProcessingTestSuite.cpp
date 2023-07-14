#include <gtest/gtest.h>

#include "IncludeDirective.h"
#include "IncludeProcessing.h"
#include "TextAccess.h"
#include "language_packages.h"
#include "utility.h"

TEST(CxxIncludeProcessing, includeDetectionFindsIncludeWithQuotes) {
  std::vector<IncludeDirective> includeDirectives = IncludeProcessing::getIncludeDirectives(
      TextAccess::createFromString("#include \"foo.h\"", FilePath(L"foo.cpp")));

  EXPECT_TRUE(!includeDirectives.empty());

  if(!includeDirectives.empty()) {
    EXPECT_TRUE(L"foo.h" == includeDirectives.front().getIncludedFile().wstr());
    EXPECT_TRUE(L"foo.cpp" == includeDirectives.front().getIncludingFile().wstr());
  }
}

TEST(CxxIncludeProcessing, includeDetectionFindsIncludeWithAngleBrackets) {
  std::vector<IncludeDirective> includeDirectives = IncludeProcessing::getIncludeDirectives(
      TextAccess::createFromString("#include <foo.h>", FilePath(L"foo.cpp")));

  EXPECT_TRUE(!includeDirectives.empty());

  if(!includeDirectives.empty()) {
    EXPECT_TRUE(L"foo.h" == includeDirectives.front().getIncludedFile().wstr());
    EXPECT_TRUE(L"foo.cpp" == includeDirectives.front().getIncludingFile().wstr());
  }
}

TEST(CxxIncludeProcessing, includeDetectionFindsIncludeWithQuotesAndSpaceBeforeKeyword) {
  std::vector<IncludeDirective> includeDirectives = IncludeProcessing::getIncludeDirectives(
      TextAccess::createFromString("# include \"foo.h\"", FilePath(L"foo.cpp")));

  EXPECT_TRUE(!includeDirectives.empty());

  if(!includeDirectives.empty()) {
    EXPECT_TRUE(L"foo.h" == includeDirectives.front().getIncludedFile().wstr());
    EXPECT_TRUE(L"foo.cpp" == includeDirectives.front().getIncludingFile().wstr());
  }
}

TEST(CxxIncludeProcessing, includeDetectionDoesNotFindIncludeInEmptyFile) {
  EXPECT_TRUE(IncludeProcessing::getIncludeDirectives(TextAccess::createFromString("")).empty());
}

TEST(CxxIncludeProcessing, includeDetectionDoesNotFindIncludeInFileWithoutPreprocessorDirective) {
  EXPECT_TRUE(IncludeProcessing::getIncludeDirectives(TextAccess::createFromString("foo")).empty());
}

TEST(CxxIncludeProcessing, includeDetectionDoesNotFindIncludeInFileWithoutIncludePreprocessorDirective) {
  EXPECT_TRUE(IncludeProcessing::getIncludeDirectives(TextAccess::createFromString("#ifdef xx\n#endif")).empty());
}

TEST(CxxIncludeProcessing, headerSearchPathDetectionDoesNotFindPathRelativeToIncludingFile) {
  std::vector<FilePath> headerSearchDirectories = utility::toVector(IncludeProcessing::getHeaderSearchDirectories(
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_does_not_find_path_relative_to_including_"
                L"file/a.cpp")},
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_does_not_find_path_relative_to_including_"
                L"file")},
      {},
      1,
      [](float) {}));

  EXPECT_TRUE(headerSearchDirectories.empty());
}

TEST(CxxIncludeProcessing, headerSearchPathDetectionFindsPathInsideSubDirectory) {
  std::vector<FilePath> headerSearchDirectories = utility::toVector(IncludeProcessing::getHeaderSearchDirectories(
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_inside_sub_directory/a.cpp")},
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_inside_sub_directory")},
      {},
      1,
      [](float) {}));

  EXPECT_TRUE(
      utility::containsElement<FilePath>(headerSearchDirectories,
                                         FilePath(L"data/CxxIncludeProcessingTestSuite/"
                                                  L"test_header_search_path_detection_finds_path_inside_sub_directory/include")
                                             .makeAbsolute()));
}

TEST(CxxIncludeProcessing, headerSearchPathDetectionFindsPathRelativeToSubDirectory) {
  std::vector<FilePath> headerSearchDirectories = utility::toVector(IncludeProcessing::getHeaderSearchDirectories(
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_relative_to_sub_directory/"
                L"a.cpp")},
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_relative_to_sub_directory")},
      {},
      1,
      [](float) {}));

  EXPECT_TRUE(utility::containsElement<FilePath>(
      headerSearchDirectories,
      FilePath(L"data/CxxIncludeProcessingTestSuite/"
               L"test_header_search_path_detection_finds_path_relative_to_sub_directory/include")
          .makeAbsolute()));
}

TEST(CxxIncludeProcessing, headerSearchPathDetectionFindsPathIncludedInHeaderSearchPath) {
  std::vector<FilePath> headerSearchDirectories = utility::toVector(IncludeProcessing::getHeaderSearchDirectories(
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_included_in_header_search_"
                L"path/a.cpp")},
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_included_in_header_search_"
                L"path/include_b")},
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_included_in_header_search_"
                L"path/include_a")},
      1,
      [](float) {}));


  EXPECT_TRUE(
      utility::containsElement<FilePath>(headerSearchDirectories,
                                         FilePath(L"data/CxxIncludeProcessingTestSuite/"
                                                  L"test_header_search_path_detection_finds_path_included_in_header_search_path/"
                                                  L"include_b")
                                             .makeAbsolute()));
}

TEST(CxxIncludeProcessing, headerSearchPathDetectionFindsPathIncludedInFutureHeaderSearchPath) {
  std::vector<FilePath> headerSearchDirectories = utility::toVector(IncludeProcessing::getHeaderSearchDirectories(
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_included_in_future_header_"
                L"search_path/a.cpp")},
      {FilePath(L"data/CxxIncludeProcessingTestSuite/"
                L"test_header_search_path_detection_finds_path_included_in_future_header_"
                L"search_path")},
      {},
      1,
      [](float) {}));

  EXPECT_TRUE(utility::containsElement<FilePath>(
      headerSearchDirectories,
      FilePath(L"data/CxxIncludeProcessingTestSuite/"
               L"test_header_search_path_detection_finds_path_included_in_future_header_search_"
               L"path/include_a")
          .makeAbsolute()));
  EXPECT_TRUE(utility::containsElement<FilePath>(
      headerSearchDirectories,
      FilePath(L"data/CxxIncludeProcessingTestSuite/"
               L"test_header_search_path_detection_finds_path_included_in_future_header_search_"
               L"path/include_b")
          .makeAbsolute()));
}