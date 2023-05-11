// catch2
#include <catch2/catch_all.hpp>
// internal
#include "globalStrings.h"

constexpr bool strings_equal(char const* value0, char const* value1) {
  return std::string_view(value0) == value1;
}

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("get github url", "[lib_gui]") {
  constexpr auto URL = "github"_g;
  STATIC_CHECK(std::is_same_v<const char* const, decltype(URL)>);
  STATIC_CHECK(strings_equal(URL, "https://github.com/OpenSourceSourceTrail/Sourcetrail"));
}