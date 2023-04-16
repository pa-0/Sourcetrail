// catch2
#include <catch2/catch_all.hpp>
// internal
#include "FilePathFilter.h"

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("file path filter finds exact match", "[core]") {
  FilePathFilter filter(L"test.h");

  REQUIRE(filter.isMatching(FilePath(L"test.h")));
}

TEST_CASE("file path filter finds match with single asterisk in same level", "[core]") {
  FilePathFilter filter(L"*test.*");

  REQUIRE(filter.isMatching(FilePath(L"this_is_a_test.h")));
}

TEST_CASE("file path filter finds match with single asterisk in different level", "[core]") {
  FilePathFilter filter(L"*/this_is_a_test.h");

  REQUIRE(filter.isMatching(FilePath(L"folder/this_is_a_test.h")));
}

TEST_CASE("file path filter does not find match with single asterisk in different level", "[core]") {
  FilePathFilter filter(L"*/test.h");

  REQUIRE(!filter.isMatching(FilePath(L"test.h")));
}

TEST_CASE("file path filter finds match with multiple asterisk in same level", "[core]") {
  FilePathFilter filter(L"**test.h");

  REQUIRE(filter.isMatching(FilePath(L"folder/this_is_a_test.h")));
}

TEST_CASE("file path filter finds match with multiple asterisk in different level", "[core]") {
  FilePathFilter filter(L"root/**/test.h");

  REQUIRE(filter.isMatching(FilePath(L"root/folder1/folder2/test.h")));
}

TEST_CASE("file path filter does not find match with multiple asterisk in different level", "[core]") {
  FilePathFilter filter(L"**/test.h");

  REQUIRE(!filter.isMatching(FilePath(L"folder/this_is_a_test.h")));
}

TEST_CASE("file path filter escapes dot character", "[core]") {
  FilePathFilter filter(L"test.h");

  REQUIRE(!filter.isMatching(FilePath(L"testyh")));
}

TEST_CASE("file path filter escapes plus character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test+.h").isMatching(FilePath(L"folder/test+.h")));
}

TEST_CASE("file path filter escapes minus character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test[-].h").isMatching(FilePath(L"folder/test[-].h")));
}

TEST_CASE("file path filter escapes dollar character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test$.h").isMatching(FilePath(L"folder/test$.h")));
}

TEST_CASE("file path filter escapes circumflex character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test^.h").isMatching(FilePath(L"folder/test^.h")));
}

TEST_CASE("file path filter escapes open round brace character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test(.h").isMatching(FilePath(L"folder/test(.h")));
}

TEST_CASE("file path filter escapes close round brace character", "[core]") {
  REQUIRE(FilePathFilter(L"folder\\test).h").isMatching(FilePath(L"folder/test).h")));
}

TEST_CASE("file path filter escapes open curly brace character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test{.h").isMatching(FilePath(L"folder/test{.h")));
}

TEST_CASE("file path filter escapes close curly brace character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test}.h").isMatching(FilePath(L"folder/test}.h")));
}

TEST_CASE("file path filter escapes open squared brace character", "[core]") {
  REQUIRE(FilePathFilter(L"folder/test[.h").isMatching(FilePath(L"folder/test[.h")));
}

TEST_CASE("file path filter escapes close squared brace character", "[core]") {
  REQUIRE(FilePathFilter(L"folder\\test].h").isMatching(FilePath(L"folder/test].h")));
}

TEST_CASE("file path filter finds backslash if slash was provided", "[core]") {
  FilePathFilter filter(L"folder/test.h");

  REQUIRE(filter.isMatching(FilePath(L"folder\\test.h")));
}

TEST_CASE("file path filter finds slash if backslash was provided", "[core]") {
  FilePathFilter filter(L"folder\\test.h");

  REQUIRE(filter.isMatching(FilePath(L"folder/test.h")));
}
