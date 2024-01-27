#include <gtest/gtest.h>

#include "Settings.h"

namespace {
class TestSettings : public Settings {
public:
  bool getBool() const {
    return getValue<bool>("Bool", false);
  }

  bool setBool(bool value) {
    return setValue<bool>("Bool", value);
  }

  int getInt() const {
    return getValue<int>("Int", -1);
  }

  bool setInt(int value) {
    return setValue<int>("Int", value);
  }

  float getFloat() const {
    constexpr auto FloatValue = 0.01F;
    return getValue<float>("Float", FloatValue);
  }

  bool setFloat(float value) {
    return setValue<float>("Float", value);
  }

  std::string getString() const {
    return getValue<std::string>("String", "<empty>");
  }

  bool setString(const std::string& value) {
    return setValue<std::string>("String", value);
  }

  std::wstring getWString() const {
    return getValue<std::wstring>("WString", L"<empty>");
  }

  bool setWString(const std::wstring& value) {
    return setValue<std::wstring>("WString", value);
  }

  bool getNewBool() const {
    return getValue<bool>("NewBool", false);
  }

  bool setNewBool(bool value) {
    return setValue<bool>("NewBool", value);
  }
};
}    // namespace

TEST(Settings, settingsGetLoadedFromFile) {
  TestSettings settings;
  EXPECT_TRUE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));
}

TEST(Settings, settingsGetNotLoadedFromFile) {
  TestSettings settings;
  EXPECT_TRUE(!settings.load(FilePath(L"data/SettingsTestSuite/wrong_settings.xml")));
}

TEST(Settings, settingsGetLoadedValue) {
  constexpr auto FloatValue = 3.1416F;

  TestSettings settings;
  EXPECT_TRUE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  EXPECT_TRUE(settings.getBool() == true);
  EXPECT_TRUE(settings.getInt() == 42);
  EXPECT_TRUE(settings.getFloat() == FloatValue);
  EXPECT_TRUE(settings.getString() == "Hello World!");
  EXPECT_TRUE(settings.getWString() == L"Hello World!");
}

TEST(Settings, settingsGetDefaultValueWhenNotLoaded) {
  constexpr auto FloatValue = 0.01F;

  const TestSettings settings;
  EXPECT_TRUE(settings.getBool() == false);
  EXPECT_TRUE(settings.getInt() == -1);
  EXPECT_TRUE(settings.getFloat() == FloatValue);
  EXPECT_TRUE(settings.getString() == "<empty>");
  EXPECT_TRUE(settings.getWString() == L"<empty>");
}

TEST(Settings, settingsGetDefaultValueWhenWronglyLoaded) {
  constexpr auto FloatValue = 0.01F;

  TestSettings settings;
  EXPECT_TRUE(!settings.load(FilePath(L"data/SettingsTestSuite/wrong_settings.xml")));

  EXPECT_TRUE(settings.getBool() == false);
  EXPECT_TRUE(settings.getInt() == -1);
  EXPECT_TRUE(settings.getFloat() == FloatValue);
  EXPECT_TRUE(settings.getString() == "<empty>");
  EXPECT_TRUE(settings.getWString() == L"<empty>");
}

TEST(Settings, settingsGetDefaultValueAfterClearing) {
  constexpr auto FloatValue = 0.01F;

  TestSettings settings;
  EXPECT_TRUE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  settings.clear();
  EXPECT_TRUE(settings.getBool() == false);
  EXPECT_TRUE(settings.getInt() == -1);
  EXPECT_TRUE(settings.getFloat() == FloatValue);
  EXPECT_TRUE(settings.getString() == "<empty>");
  EXPECT_TRUE(settings.getWString() == L"<empty>");
}

TEST(Settings, settingsCanBeSetWhenNotLoaded) {
  constexpr auto FloatValue = 2.5F;

  TestSettings settings;

  EXPECT_TRUE(settings.setBool(false));
  EXPECT_TRUE(settings.getBool() == false);

  EXPECT_TRUE(settings.setInt(2));
  EXPECT_TRUE(settings.getInt() == 2);

  EXPECT_TRUE(settings.setFloat(FloatValue));
  EXPECT_TRUE(settings.getFloat() == FloatValue);

  EXPECT_TRUE(settings.setString("foo"));
  EXPECT_TRUE(settings.getString() == "foo");

  EXPECT_TRUE(settings.setWString(L"bar"));
  EXPECT_TRUE(settings.getWString() == L"bar");
}

TEST(Settings, settingsCanBeReplacedWhenLoaded) {
  constexpr auto FloatValue = 2.5F;

  TestSettings settings;
  EXPECT_TRUE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  EXPECT_TRUE(settings.setBool(false));
  EXPECT_TRUE(settings.getBool() == false);

  EXPECT_TRUE(settings.setInt(2));
  EXPECT_TRUE(settings.getInt() == 2);

  EXPECT_TRUE(settings.setFloat(FloatValue));
  EXPECT_TRUE(settings.getFloat() == FloatValue);

  EXPECT_TRUE(settings.setString("foo"));
  EXPECT_TRUE(settings.getString() == "foo");

  EXPECT_TRUE(settings.setWString(L"bar"));
  EXPECT_TRUE(settings.getWString() == L"bar");
}

TEST(Settings, settingsCanBeAddedWhenLoaded) {
  TestSettings settings;
  EXPECT_TRUE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  EXPECT_TRUE(settings.getNewBool() == false);
  EXPECT_TRUE(settings.setNewBool(true));
  EXPECT_TRUE(settings.getNewBool() == true);
}
