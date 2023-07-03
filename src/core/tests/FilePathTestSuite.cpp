// catch2
#include <catch2/catch_all.hpp>
// internal
#include "FilePath.h"

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("file_path_gets_created_empty", "[core]") {
  const FilePath path;

  REQUIRE(path.wstr().empty());
}

TEST_CASE("file_path_gets_created_with_string", "[core]") {
  const std::wstring str(L"data/FilePathTestSuite/main.cpp");
  const FilePath path(str);

  REQUIRE(path.wstr() == str);
}

TEST_CASE("file_path_gets_created_other_file_path", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/main.cpp");
  const FilePath path2(path); // NOLINT(performance-unnecessary-copy-initialization)

  REQUIRE(path == path2);
}

TEST_CASE("file_path_empty", "[core]") {
  const FilePath path1(L"data/FilePathTestSuite/a.cpp");
  const FilePath path2;

  REQUIRE(!path1.empty());
  REQUIRE(path2.empty());
}

TEST_CASE("file_path_exists", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.cpp");

  REQUIRE(path.exists());
}

TEST_CASE("file_path_not_exists", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.h");

  REQUIRE(!path.exists());
}

TEST_CASE("file_path_is_directory", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.cpp");

  REQUIRE(!path.isDirectory());
  REQUIRE(path.getParentDirectory().isDirectory());
}

TEST_CASE("empty_file_path_has_empty_parent_directory", "[core]") {
  const FilePath path;

  REQUIRE(path.empty());
  REQUIRE(path.getParentDirectory().empty());
}

TEST_CASE("file_path_without_parent_has_empty_parent_directory", "[core]") {
  const FilePath path(L"a.cpp");

  REQUIRE(path.getParentDirectory().empty());
}

TEST_CASE("file_path_is_absolute", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.cpp");

  REQUIRE(!path.isAbsolute());
  REQUIRE(path.getAbsolute().isAbsolute());
}

TEST_CASE("file_path_parent_directory", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.cpp");

  REQUIRE(path.getParentDirectory().wstr() == L"data/FilePathTestSuite");
  REQUIRE(path.getParentDirectory().getParentDirectory().wstr() == L"data");
}

TEST_CASE("file_path_relative_to_other_path", "[core]") {
  const FilePath pathA(L"data/FilePathTestSuite/a.cpp");
  const FilePath pathB(L"data/FilePathTestSuite/test/c.h");

  REQUIRE(pathA.getRelativeTo(pathB).wstr() == L"../a.cpp");
  REQUIRE(pathB.getRelativeTo(pathA).wstr() == L"test/c.h");
}

TEST_CASE("file_path_relative_to_other_directory", "[core]") {
  const FilePath pathA(L"data/FilePathTestSuite/a.cpp");
  const FilePath pathB(L"data/FilePathTestSuite/test");

  REQUIRE(pathA.getRelativeTo(pathB).wstr() == L"../a.cpp");
}

TEST_CASE("file_path_relative_to_same_directory", "[core]") {
  const FilePath pathA(L"data/FilePathTestSuite/test");

  REQUIRE(pathA.getRelativeTo(pathA).wstr() == L"./");
}

TEST_CASE("file_path_file_name", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/abc.h");

  REQUIRE(path.fileName() == L"abc.h");
}

TEST_CASE("file_path_extension", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.h");

  REQUIRE(path.extension() == L".h");
}

TEST_CASE("file_path_without_extension", "[core]") {
  const FilePath path(L"data/FilePathTestSuite/a.h");

  REQUIRE(path.withoutExtension() == FilePath(L"data/FilePathTestSuite/a"));
}

TEST_CASE("file_path_has_extension", "[core]") {
  std::vector<std::wstring> extensions;
  extensions.emplace_back(L".h");
  extensions.emplace_back(L".cpp");
  extensions.emplace_back(L".cc");

  REQUIRE(FilePath(L"data/FilePathTestSuite/a.h").hasExtension(extensions));
  REQUIRE(FilePath(L"data/FilePathTestSuite/b.cpp").hasExtension(extensions));
  REQUIRE(!FilePath(L"data/FilePathTestSuite/a.m").hasExtension(extensions));
}

TEST_CASE("file_path_equals_file_with_different_relative_paths", "[core]") {
  const FilePath path1(L"data/FilePathTestSuite/a.cpp");
  const FilePath path2(L"data/../data/FilePathTestSuite/./a.cpp");

  REQUIRE(path1 == path2);
}

TEST_CASE("file_path_equals_relative_and_absolute_paths", "[core]") {
  const FilePath path1(L"data/FilePathTestSuite/a.cpp");
  const FilePath path2 = path1.getAbsolute();

  REQUIRE(path1 == path2);
}

TEST_CASE("file_path_equals_absolute_and_canonical_paths", "[core]") {
  const FilePath path(L"data/../data/FilePathTestSuite/./a.cpp");

  REQUIRE(path.getAbsolute() == path.getCanonical());
}

#if 0
TEST_CASE("file_path_canonical_removes_symlinks", "[core]") {
#ifndef _WIN32
  const FilePath pathA(L"data/FilePathTestSuite/parent/target/d.cpp");
  const FilePath pathB(L"data/FilePathTestSuite/target/d.cpp");

  REQUIRE(pathB.getAbsolute() == pathA.getCanonical());
#endif
}
#endif

TEST_CASE("file_path_compares_paths_with_posix_and_windows_format", "[core]") {
#ifdef _WIN32
  const FilePath pathB(L"data/FilePathTestSuite/b.cc");
  const FilePath pathB2(L"data\\FilePathTestSuite\\b.cc");

  REQUIRE(pathB == pathB2);
#endif
}

TEST_CASE("file_path_differs_for_different_existing_files", "[core]") {
  const FilePath pathA(L"data/FilePathTestSuite/a.cpp");
  const FilePath pathB(L"data/FilePathTestSuite/b.cc");

  REQUIRE(pathA != pathB);
}

TEST_CASE("file_path_differs_for_different_nonexistent_files", "[core]") {
  const FilePath pathA(L"data/FilePathTestSuite/a.h");
  const FilePath pathB(L"data/FilePathTestSuite/b.c");

  REQUIRE(pathA != pathB);
}

TEST_CASE("file_path_differs_for_existing_and_nonexistent_files", "[core]") {
  const FilePath pathA(L"data/FilePathTestSuite/a.h");
  const FilePath pathB(L"data/FilePathTestSuite/b.cc");

  REQUIRE(pathA != pathB);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("file path is valid for absolute and relative existing files and directories paths", "[core]") {
  REQUIRE(FilePath(L"data/FilePathTestSuite/a.cpp").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/a.cpp").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/with space/s.srctrlprj").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/with space/s.srctrlprj").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/container.app").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/container.app").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/container.app/b.txt").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/container.app/b.txt").makeAbsolute().isValid());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("file path is valid for absolute and relative non-existing files and directories paths", "[core]") {
  REQUIRE(FilePath(L"data/non-existing-file.cpp").isValid());
  REQUIRE(FilePath(L"data/non-existing-file.cpp").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/non-existing-dir").isValid());
  REQUIRE(FilePath(L"data/non-existing-dir").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/container.app/c.txt").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/container.app/c.txt").makeAbsolute().isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/also space").isValid());
  REQUIRE(FilePath(L"data/FilePathTestSuite/also space").makeAbsolute().isValid());
}

TEST_CASE("file path is invalid for absolute and relative paths with invalid characters", "[core]") {
  REQUIRE(!FilePath(L"data/non-exis\"ting-file.cpp").isValid());
  REQUIRE(!FilePath(L"data/non-exis\"ting-file.cpp").makeAbsolute().isValid());
  REQUIRE(!FilePath(L"data/non-exis|ting-dir").isValid());
  REQUIRE(!FilePath(L"data/non-exis|ting-dir").makeAbsolute().isValid());
  REQUIRE(!FilePath(L"data/FilePathTestSuite/container:app").isValid());
  REQUIRE(!FilePath(L"data/FilePathTestSuite/container:app").makeAbsolute().isValid());
}
