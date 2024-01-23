#include <memory>
#include <string>

#include <gtest/gtest.h>

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

TEST(ConfigManager, returnsTrueWhenKeyIsFound) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success = config->getValue("path/to/single_value", value);

  EXPECT_TRUE(success);
}

TEST(ConfigManager, returnsFalseWhenKeyIsNotFound) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success = config->getValue("path/to/nowhere", value);

  EXPECT_TRUE(!success);
}

TEST(ConfigManager, returnsCorrectStringForKey) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  std::wstring value;
  config->getValue("path/to/single_value", value);

  EXPECT_TRUE(L"42" == value);
}

TEST(ConfigManager, returnsCorrectFloatForKey) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  config->getValue("path/to/single_value", value);

  EXPECT_FLOAT_EQ(value, 42.0F);
}

TEST(ConfigManager, returnsCorrectBoolForKeyIfValueIsTrue) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success(config->getValue("path/to/bool_that_is_true", value));

  EXPECT_TRUE(success);
  EXPECT_TRUE(value);
}

TEST(ConfigManager, returnsCorrectBoolForKeyIfValueIsFalse) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  float value = 0;
  bool success(config->getValue("path/to/bool_that_is_false", value));

  EXPECT_TRUE(success);
  EXPECT_TRUE(!value);
}

TEST(ConfigManager, addsNewKeyWhenEmpty) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createEmpty();

  config->setValue("path/to/true_bool", true);

  bool value = false;
  bool success(config->getValue("path/to/true_bool", value));

  EXPECT_TRUE(success);
  EXPECT_TRUE(value);
}

TEST(ConfigManager, addsNewKeyWhenNotEmpty) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  config->setValue("path/to/true_bool", true);

  bool value = false;
  bool success(config->getValue("path/to/true_bool", value));

  EXPECT_TRUE(success);
  EXPECT_TRUE(value);
}

TEST(ConfigManager, returnsCorrectListForKey) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());

  std::vector<int> values;

  bool success(config->getValues("paths/path", values));

  EXPECT_TRUE(success);
  EXPECT_TRUE(values.size() == 3);
  EXPECT_TRUE(values[0] == 2);
  EXPECT_TRUE(values[1] == 5);
  EXPECT_TRUE(values[2] == 8);
}

TEST(ConfigManager, saveAndLoadConfigurationAndCompare) {
  const FilePath path(L"data/ConfigManagerTestSuite/temp.xml");

  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(getConfigTextAccess());
  config->save(path.str());
  std::shared_ptr<ConfigManager> config2 = ConfigManager::createAndLoad(TextAccess::createFromFile(path));
  EXPECT_TRUE(config->toString() == config2->toString());
}

TEST(ConfigManager, loadsSpecialCharacter) {
  std::shared_ptr<ConfigManager> config = ConfigManager::createAndLoad(
      TextAccess::createFromFile(FilePath(L"data/ConfigManagerTestSuite/test_data.xml")));
  std::wstring loadedSpecialCharacter;
  config->getValue("path/to/special_character", loadedSpecialCharacter);

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
