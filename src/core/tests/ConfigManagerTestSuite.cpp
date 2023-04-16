// STL
#include <memory>
#include <string>
// catch2
#include <catch2/catch_all.hpp>
// internal
#include "ConfigManager.h"
#include "TextAccess.h"

namespace {
std::shared_ptr<TextAccess> getConfigTextAccess() {
  const std::string text =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
      "<config>\n"
      "	<path>\n"
      "		<to>\n"
      "			<bool_that_is_false>0</bool_that_is_false>\n"
      "			<bool_that_is_true>1</bool_that_is_true>\n"
      "			<single_value>42</single_value>\n"
      "		</to>\n"
      "	</path>\n"
      "	<paths>\n"
      "		<nopath>4</nopath>\n"
      "		<path>2</path>\n"
      "		<path>5</path>\n"
      "		<path>8</path>\n"
      "	</paths>\n"
      "</config>\n";
  return TextAccess::createFromString(text);
}
}    // namespace

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("config manager returns true when key is found", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success = config->getValue("path/to/single_value", value);

  REQUIRE(success);
}

TEST_CASE("config manager returns false when key is not found", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success = config->getValue("path/to/nowhere", value);

  REQUIRE(!success);
}

TEST_CASE("config manager returns correct string for key", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  std::wstring value;
  config->getValue("path/to/single_value", value);

  REQUIRE(L"42" == value);
}

TEST_CASE("config manager returns correct float for key", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  config->getValue("path/to/single_value", value);

  REQUIRE(value == Catch::Approx(42.0F));
}

TEST_CASE("config manager returns correct bool for key if value is true", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success(config->getValue("path/to/bool_that_is_true", value));

  REQUIRE(success);
  REQUIRE(value);
}

TEST_CASE("config manager returns correct bool for key if value is false", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success(config->getValue("path/to/bool_that_is_false", value));

  REQUIRE(success);
  REQUIRE(!value);
}

TEST_CASE("config manager adds new key when empty", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createEmpty();

  config->setValue("path/to/true_bool", true);

  bool value = false;
  bool success(config->getValue("path/to/true_bool", value));

  REQUIRE(success);
  REQUIRE(value);
}

TEST_CASE("config manager adds new key when not empty", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  config->setValue("path/to/true_bool", true);

  bool value = false;
  bool success(config->getValue("path/to/true_bool", value));

  REQUIRE(success);
  REQUIRE(value);
}

TEST_CASE("config manager returns correct list for key", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  std::vector<int> values;

  bool success(config->getValues("paths/path", values));

  REQUIRE(success);
  REQUIRE(values.size() == 3);
  REQUIRE(values[0] == 2);
  REQUIRE(values[1] == 5);
  REQUIRE(values[2] == 8);
}

TEST_CASE("config manager save and load configuration and compare", "[core]") {
  const FilePath path(L"data/ConfigManagerTestSuite/temp.xml");

  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());
  config->save(path.str());
  std::shared_ptr<ConfigManager> config2 = ConfigManager::createAndLoad(
      TextAccess::createFromFile(path));
  REQUIRE(config->toString() == config2->toString());
}

TEST_CASE("config manager loads special character", "[core]") {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(
      TextAccess::createFromFile(FilePath(L"data/ConfigManagerTestSuite/test_data.xml")));
  std::wstring loadedSpecialCharacter;
  config->getValue("path/to/special_character", loadedSpecialCharacter);

  REQUIRE(loadedSpecialCharacter.size() == 1);
  REQUIRE(loadedSpecialCharacter[0] == wchar_t(252));
  ;    // special character needs to be encoded as ASCII code because
       // otherwise cxx compiler may be complaining
}

TEST_CASE("config manager save and load special character and compare", "[core]") {
  const FilePath path(L"data/ConfigManagerTestSuite/temp.xml");
  std::wstring specialCharacter;
  specialCharacter.push_back(wchar_t(252));

  std::shared_ptr<ConfigManager> config = ConfigManager::createEmpty();
  config->setValue("path/to/special_character", specialCharacter);
  config->save(path.str());

  std::shared_ptr<ConfigManager> config2 = ConfigManager::createAndLoad(
      TextAccess::createFromFile(path));
  REQUIRE(config->toString() == config2->toString());
}
