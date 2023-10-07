// gtest
#include <gtest/gtest.h>
// internal
#include "globalStrings.h"

constexpr bool strings_equal(char const* value0, char const* value1) {
  return std::string_view(value0) == value1;
}

// NOLINTNEXTLINE(cert-err58-cpp)
TEST(getGithubUrl, goodCase) {
  constexpr auto URL = "github"_g;
  static_assert( std::is_same_v<const char* const, decltype(URL)>);
  EXPECT_EQ(URL, "https://github.com/OpenSourceSourceTrail/Sourcetrail");
}