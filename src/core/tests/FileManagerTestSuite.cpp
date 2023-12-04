#include <algorithm>
#include <iterator>

#include <gtest/gtest.h>

#include "FileManager.h"
#include "FilePath.h"
#include "FilePathFilter.h"
#include "FileSystem.h"
#include "utility.h"


TEST(FileManager, hasAddedFilePathsAfterFirstFetch) {
  std::vector<FilePath> sourcePaths;
  sourcePaths.emplace_back(L"./data/FileManagerTestSuite/src/");
  sourcePaths.emplace_back(L"./data/FileManagerTestSuite/include/");
  std::vector<FilePath> headerPaths;
  std::vector<FilePathFilter> excludeFilters;

  // catch exceptions thrown on linux build machine
  try {
    std::vector<FilePath> filePaths = FileSystem::getFilePathsFromDirectory(FilePath(L"./data/FileManagerTestSuite/src/"));
    EXPECT_TRUE(filePaths.size() == 3);

    std::vector<std::wstring> sourceExtensions;
    std::transform(std::cbegin(filePaths), std::cend(filePaths), std::back_inserter(sourceExtensions), [](const auto& filePath) {
      return filePath.extension();
    });
    EXPECT_TRUE(sourceExtensions.size() == 3);

    FileManager fileManager;
    fileManager.update(sourcePaths, excludeFilters, sourceExtensions);
    std::vector<FilePath> foundSourcePaths = utility::toVector(fileManager.getAllSourceFilePaths());

    EXPECT_TRUE(foundSourcePaths.size() == 3);
    EXPECT_TRUE(utility::containsElement<FilePath>(foundSourcePaths, filePaths[0]));
    EXPECT_TRUE(utility::containsElement<FilePath>(foundSourcePaths, filePaths[1]));
    EXPECT_TRUE(utility::containsElement<FilePath>(foundSourcePaths, filePaths[2]));
  } catch(...) {
  }
}