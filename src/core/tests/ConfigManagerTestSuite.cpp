#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "ConfigManager.h"
#include "TextAccess.h"

namespace {
std::shared_ptr<TextAccess> getConfigTextAccess() {
  static constexpr std::string_view text =
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
  return TextAccess::createFromString(text.data());
}
}    // namespace

struct ConfigManagerFix : testing::Test {
  void SetUp() override {
    configManager = ConfigManager::createAndLoad(getConfigTextAccess());
  }

  ConfigManager::Ptr configManager;
};

TEST_F(ConfigManagerFix, returnsTrueWhenKeyIsFound) {
  const auto result = configManager->getValue<float>("path/to/single_value");
  ASSERT_TRUE(result);
  EXPECT_FLOAT_EQ(42, result.value());
}

TEST_F(ConfigManagerFix, returnsFalseWhenKeyIsNotFound) {
  EXPECT_FALSE(configManager->getValue<float>("path/to/nowhere"));
}

TEST_F(ConfigManagerFix, returnsCorrectFloatForKey) {
  const auto result = configManager->getValue<int>("path/to/single_value");
  ASSERT_TRUE(result);
  EXPECT_EQ(42, result.value());
}

TEST_F(ConfigManagerFix, returnsCorrectBoolForKeyIfValueIsTrue) {
  const auto result = configManager->getValue<float>("path/to/bool_that_is_true");
  ASSERT_TRUE(result);
  EXPECT_FLOAT_EQ(1, result.value());
}

TEST_F(ConfigManagerFix, returnsCorrectBoolForKeyIfValueIsFalse) {
  const auto result = configManager->getValue<float>("path/to/bool_that_is_false");
  ASSERT_TRUE(result);
  EXPECT_FLOAT_EQ(0, result.value());
}

TEST_F(ConfigManagerFix, addsNewKeyWhenNotEmpty) {
  configManager->setValue("path/to/true_bool", true);

  const auto result = configManager->getValue<bool>("path/to/true_bool");
  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
}

TEST_F(ConfigManagerFix, returnsCorrectListForKey) {
  auto result = configManager->getValues<int>("paths/path");
  ASSERT_TRUE(result.has_value());

  auto values = result.value();
  ASSERT_EQ(3, values.size());
  EXPECT_TRUE(values[0] == 2);
  EXPECT_TRUE(values[1] == 5);
  EXPECT_TRUE(values[2] == 8);
}

TEST_F(ConfigManagerFix, saveAndLoadConfigurationAndCompare) {
  constexpr std::string_view path = "data/ConfigManagerTestSuite/temp.xml";
  configManager->save(path);

  std::shared_ptr<ConfigManager> config2 = ConfigManager::createAndLoad(TextAccess::createFromFile(FilePath {path.data()}));
  EXPECT_TRUE(configManager->toString() == config2->toString());
}

TEST(ConfigManager, loadsSpecialCharacter) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(
      TextAccess::createFromFile(FilePath(L"data/ConfigManagerTestSuite/test_data.xml")));
  auto result = config->getValue<std::wstring>("path/to/special_character");

  ASSERT_TRUE(result);
  std::wstring loadedSpecialCharacter = result.value();
  EXPECT_TRUE(loadedSpecialCharacter.size() == 1);
  EXPECT_TRUE(loadedSpecialCharacter[0] == wchar_t(252));
  ;    // special character needs to be encoded as ASCII code because
       // otherwise cxx compiler may be complaining
}

TEST(ConfigManager, saveAndLoadSpecialCharacterAndCompare) {
  const FilePath path(L"data/ConfigManagerTestSuite/temp.xml");
  std::wstring specialCharacter;
  specialCharacter.push_back(wchar_t(252));

  std::shared_ptr<ConfigManager> config = ConfigManager::createEmpty();
  config->setValue("path/to/special_character", specialCharacter);
  config->save(path.str());

  std::shared_ptr<ConfigManager> config2 = ConfigManager::createAndLoad(TextAccess::createFromFile(path));
  EXPECT_TRUE(config->toString() == config2->toString());
}

TEST(ConfigManager, addsNewKeyWhenEmpty) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createEmpty();

  config->setValue("path/to/true_bool", true);

  const auto result = config->getValue<bool>("path/to/true_bool");

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
}