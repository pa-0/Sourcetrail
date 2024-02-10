#pragma once
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "FilePath.h"
#include "utilityString.h"

class TextAccess;
class TiXmlNode;

class ConfigManager final {
public:
  using Ptr = std::shared_ptr<ConfigManager>;

  static Ptr createEmpty();

  static Ptr createAndLoad(const std::shared_ptr<TextAccess>& textAccess);

  template <typename T>
  static T fromString(const std::string& value) {
    if constexpr(std::is_same_v<T, std::string>) {
      return value;
    } else if constexpr(std::is_same_v<T, std::wstring>) {
      return utility::decodeFromUtf8(value);
    } else if constexpr(std::is_same_v<T, int>) {
      try {
        return std::stoi(value);
      } catch(const std::invalid_argument& ex) {
      }
    } else if constexpr(std::is_same_v<T, float>) {
      try {
        return std::stof(value);
      } catch(const std::invalid_argument& ex) {
      }
    } else if constexpr(std::is_same_v<T, bool>) {
      try {
        return std::stoi(value) != 0;
      } catch(const std::invalid_argument& ex) {
      }
    } else if constexpr(std::is_same_v<T, FilePath>) {
      return FilePath {value};
    }
    return {};
  }

  template <typename T>
  static std::string toString(const T& value) {
    std::string valueString;
    if constexpr(std::is_same_v<T, std::string>) {
      valueString = value;
    } else if constexpr(std::is_same_v<T, std::wstring>) {
      valueString = utility::encodeToUtf8(value);
    } else if constexpr(std::is_same_v<T, int> || std::is_same_v<T, float>) {
      valueString = std::to_string(value);
    } else if constexpr(std::is_same_v<T, bool>) {
      valueString = value ? '1' : '0';
    } else if constexpr(std::is_same_v<T, FilePath>) {
      valueString = value.str();
    }
    return valueString;
  }

  ConfigManager(ConfigManager&&) = delete;
  ConfigManager& operator=(const ConfigManager&) = delete;
  ConfigManager& operator=(ConfigManager&&) = delete;

  ~ConfigManager();

  Ptr createCopy();

  void clear();

  template <typename T = std::string>
  [[nodiscard]] std::optional<T> getValue(const std::string& key) const;

  template <typename T>
  T getValueOrDefault(const std::string& key, T defaultValue) const;

  template <typename T = std::string>
  [[nodiscard]] std::optional<std::vector<T>> getValues(const std::string& key) const;

  template <typename T>
  std::vector<T> getValuesOrDefaults(const std::string& key, std::vector<T> defaultValues) const;

  template <typename T = std::string>
  void setValue(const std::string& key, const T& value);

  template <typename T = std::string>
  void setValues(const std::string& key, const std::vector<T>& values);

  void removeValues(const std::string& key);

  bool isValueDefined(const std::string& key) const;
  std::vector<std::string> getSublevelKeys(const std::string& key) const;

  bool load(const std::shared_ptr<TextAccess>& textAccess);
  bool save(std::string_view filepath);
  std::string toString();

  void setWarnOnEmptyKey(bool warnOnEmptyKey) const;

private:
  ConfigManager();
  ConfigManager(const ConfigManager&);

  void parseSubtree(TiXmlNode* parentElement, const std::string& currentPath);
  bool createXmlDocument(bool saveAsFile, std::string_view filepath, std::string& output);

  std::multimap<std::string, std::string> m_values;
  mutable bool m_warnOnEmptyKey = true;
};

template <typename T>
std::optional<T> ConfigManager::getValue(const std::string& key) const {
  if(auto iterator = m_values.find(key); iterator != m_values.end()) {
    return fromString<T>(iterator->second);
  }
  return std::nullopt;
}

template <typename T>
std::optional<std::vector<T>> ConfigManager::getValues(const std::string& key) const {
  if(const auto& [first, last] = m_values.equal_range(key); first != last) {
    std::vector<T> result;
    result.reserve(static_cast<size_t>(std::distance(first, last)));
    for(auto cit = first; cit != last; ++cit) {
      result.push_back(fromString<T>(cit->second));
    }
    return result;
  }
  return std::nullopt;
}

template <typename T>
void ConfigManager::setValue(const std::string& key, const T& value) {
  auto iterator = m_values.find(key);

  std::string valueString = toString(value);
  if(iterator != m_values.end()) {
    iterator->second = valueString;
  } else {
    m_values.emplace(key, valueString);
  }
}

template <typename T>
void ConfigManager::setValues(const std::string& key, const std::vector<T>& values) {
  if(auto iterator = m_values.find(key); iterator != m_values.end()) {
    m_values.erase(key);
  }
  for(const T& value : values) {
    m_values.emplace(key, toString(value));
  }
}

template <typename T>
T ConfigManager::getValueOrDefault(const std::string& key, T defaultValue) const {
  if(auto result = getValue<T>(key); result) {
    return result.value();
  }
  return defaultValue;
}

template <typename T>
std::vector<T> ConfigManager::getValuesOrDefaults(const std::string& key, std::vector<T> defaultValues) const {
  if(auto values = getValues<T>(key); values.has_value()) {
    return values.value();
  }
  return defaultValues;
}
