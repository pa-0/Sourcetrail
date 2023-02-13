#pragma once
// STL
#include <cstddef>
#include <string_view>

constexpr const char* operator ""_g(const char* key, size_t size) noexcept {
  auto sKey = std::string_view{key, size};
  if(sKey == "github") {
    return "https://github.com/OpenSourceSourceTrail/Sourcetrail";
  }
  return "";
}