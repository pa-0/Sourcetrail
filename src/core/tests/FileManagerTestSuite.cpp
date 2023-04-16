// catch2
#include <algorithm>
#include <catch2/catch_all.hpp>
#include <iterator>
// internal
#include "FileManager.h"
#include "FilePath.h"
#include "FilePathFilter.h"
#include "FileSystem.h"
#include "utility.h"

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("file manager has added file paths after first fetch", "[core]") {
  std::vector<FilePath> sourcePaths;
  sourcePaths.emplace_back(L"./data/FileManagerTestSuite/src/");
  sourcePaths.emplace_back(L"./data/FileManagerTestSuite/include/");
  std::vector<FilePath> headerPaths;
  std::vector<FilePathFilter> excludeFilters;

  // catch exceptions thrown on linux build machine
  try {
    std::vector<FilePath> filePaths = FileSystem::getFilePathsFromDirectory(
        FilePath(L"./data/FileManagerTestSuite/src/"));
    REQUIRE(filePaths.size() == 3);

    std::vector<std::wstring> sourceExtensions;
    std::transform(std::cbegin(filePaths),
                   std::cend(filePaths),
                   std::back_inserter(sourceExtensions),
                   [](const auto& filePath) { return filePath.extension(); });
    REQUIRE(sourceExtensions.size() == 3);

    FileManager fileManager;
    fileManager.update(sourcePaths, excludeFilters, sourceExtensions);
    std::vector<FilePath> foundSourcePaths = utility::toVector(fileManager.getAllSourceFilePaths());

    REQUIRE(foundSourcePaths.size() == 3);
    REQUIRE(utility::containsElement<FilePath>(foundSourcePaths, filePaths[0]));
    REQUIRE(utility::containsElement<FilePath>(foundSourcePaths, filePaths[1]));
    REQUIRE(utility::containsElement<FilePath>(foundSourcePaths, filePaths[2]));
  } catch(...) {
  }
}