// catch2
#include <catch2/catch_all.hpp>
// internal
#include "ProjectSettings.h"
#include "SourceGroupSettings.h"
#include "SourceGroupSettingsWithSourcePaths.h"

constexpr auto SettingsPath = L"data/SettingsTestSuite/settings.xml";

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("load project settings from file", "[lib]") {
  ProjectSettings settings;
  REQUIRE(settings.load(FilePath(SettingsPath)));
}

TEST_CASE("load source path from file", "[lib]") {
  ProjectSettings projectSettings;
  projectSettings.load(FilePath(SettingsPath));
  const auto pSourceGroupSettings = std::dynamic_pointer_cast<SourceGroupSettingsWithSourcePaths>(
      projectSettings.getAllSourceGroupSettings().front());
  const auto paths = pSourceGroupSettings->getSourcePaths();

  REQUIRE(paths.size() == 2);
  REQUIRE(paths[0].wstr() == L"src");
  REQUIRE(paths[1].wstr() == L"test");
}