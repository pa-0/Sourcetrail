// catch2
#include <catch2/catch_all.hpp>
// internal
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

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("settings get loaded from file", "[lib]") {
  TestSettings settings;
  REQUIRE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));
}

TEST_CASE("settings get not loaded from file", "[lib]") {
  TestSettings settings;
  REQUIRE(!settings.load(FilePath(L"data/SettingsTestSuite/wrong_settings.xml")));
}

TEST_CASE("settings get loaded value", "[lib]") {
  constexpr auto FloatValue = 3.1416F;

  TestSettings settings;
  REQUIRE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  REQUIRE(settings.getBool()    == true);
  REQUIRE(settings.getInt()     == 42);
  REQUIRE(settings.getFloat()   == FloatValue);
  REQUIRE(settings.getString()  == "Hello World!");
  REQUIRE(settings.getWString() == L"Hello World!");
}

TEST_CASE("settings get default value when not loaded", "[lib]") {
  constexpr auto FloatValue = 0.01F;

  const TestSettings settings;
  REQUIRE(settings.getBool()    == false);
  REQUIRE(settings.getInt()     == -1);
  REQUIRE(settings.getFloat()   == FloatValue);
  REQUIRE(settings.getString()  == "<empty>");
  REQUIRE(settings.getWString() == L"<empty>");
}

TEST_CASE("settings get default value when wrongly loaded", "[lib]") {
  constexpr auto FloatValue = 0.01F;

  TestSettings settings;
  REQUIRE(!settings.load(FilePath(L"data/SettingsTestSuite/wrong_settings.xml")));

  REQUIRE(settings.getBool()    == false);
  REQUIRE(settings.getInt()     == -1);
  REQUIRE(settings.getFloat()   == FloatValue);
  REQUIRE(settings.getString()  == "<empty>");
  REQUIRE(settings.getWString() == L"<empty>");
}

TEST_CASE("settings get default value after clearing", "[lib]") {
  constexpr auto FloatValue = 0.01F;

  TestSettings settings;
  REQUIRE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  settings.clear();
  REQUIRE(settings.getBool()    == false);
  REQUIRE(settings.getInt()     == -1);
  REQUIRE(settings.getFloat()   == FloatValue);
  REQUIRE(settings.getString()  == "<empty>");
  REQUIRE(settings.getWString() == L"<empty>");
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("settings can be set when not loaded", "[lib]") {
  constexpr auto FloatValue = 2.5F;

  TestSettings settings;

  REQUIRE(settings.setBool(false));
  REQUIRE(settings.getBool() == false);

  REQUIRE(settings.setInt(2));
  REQUIRE(settings.getInt() == 2);

  REQUIRE(settings.setFloat(FloatValue));
  REQUIRE(settings.getFloat() == FloatValue);

  REQUIRE(settings.setString("foo"));
  REQUIRE(settings.getString() == "foo");

  REQUIRE(settings.setWString(L"bar"));
  REQUIRE(settings.getWString() == L"bar");
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("settings can be replaced when loaded", "[lib]") {
  constexpr auto FloatValue = 2.5F;

  TestSettings settings;
  REQUIRE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  REQUIRE(settings.setBool(false));
  REQUIRE(settings.getBool() == false);

  REQUIRE(settings.setInt(2));
  REQUIRE(settings.getInt() == 2);

  REQUIRE(settings.setFloat(FloatValue));
  REQUIRE(settings.getFloat() == FloatValue);

  REQUIRE(settings.setString("foo"));
  REQUIRE(settings.getString() == "foo");

  REQUIRE(settings.setWString(L"bar"));
  REQUIRE(settings.getWString() == L"bar");
}

TEST_CASE("settings can be added when loaded", "[lib]") {
  TestSettings settings;
  REQUIRE(settings.load(FilePath(L"data/SettingsTestSuite/settings.xml")));

  REQUIRE(settings.getNewBool() == false);
  REQUIRE(settings.setNewBool(true));
  REQUIRE(settings.getNewBool() == true);
}