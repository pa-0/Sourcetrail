#include "ConfigManager.h"

#include <set>

#include <tinyxml.h>

#include "TextAccess.h"
#include "logging.h"
#include "utility.h"
#include "utilityString.h"

std::shared_ptr<ConfigManager> ConfigManager::createEmpty() {
  return std::shared_ptr<ConfigManager>(new ConfigManager);
}

std::shared_ptr<ConfigManager> ConfigManager::createAndLoad(const std::shared_ptr<TextAccess>& textAccess) {
  auto configManager = ConfigManager::createEmpty();
  if(!configManager->load(textAccess)) {
    LOG_WARNING("Failed to load XML");
  }
  return configManager;
}

ConfigManager::ConfigManager() = default;

ConfigManager::ConfigManager(const ConfigManager&) = default;

ConfigManager::~ConfigManager() = default;

std::shared_ptr<ConfigManager> ConfigManager::createCopy() {
  return std::shared_ptr<ConfigManager>(new ConfigManager(*this));
}

void ConfigManager::clear() {
  m_values.clear();
}

void ConfigManager::removeValues(const std::string& key) {
  for(const std::string& sublevelKey : getSublevelKeys(key)) {
    removeValues(sublevelKey);
  }
  m_values.erase(key);
}

bool ConfigManager::isValueDefined(const std::string& key) const {
  return (m_values.find(key) != m_values.end());
}

std::vector<std::string> ConfigManager::getSublevelKeys(const std::string& key) const {
  std::set<std::string> keys;
  for(const auto& m_value : m_values) {
    if(utility::isPrefix(key, m_value.first)) {
      size_t startPos = m_value.first.find("/", key.size());
      if(startPos == key.size()) {
        std::string sublevelKey = m_value.first.substr(0, m_value.first.find("/", startPos + 1));
        keys.insert(sublevelKey);
      }
    }
  }
  return utility::toVector(keys);
}

bool ConfigManager::load(const std::shared_ptr<TextAccess>& textAccess) {
  TiXmlDocument doc;
  const char* pTest = doc.Parse(textAccess->getText().c_str(), nullptr, TIXML_ENCODING_UTF8);
  if(pTest != nullptr) {
    TiXmlHandle docHandle(&doc);
    TiXmlNode* rootNode = docHandle.FirstChild("config").ToNode();
    if(rootNode == nullptr) {
      LOG_ERROR("No rootelement 'config' in the configfile");
      return false;
    }
    for(TiXmlNode* childNode = rootNode->FirstChild(); childNode != nullptr; childNode = childNode->NextSibling()) {
      parseSubtree(childNode, "");
    }
  } else {
    LOG_ERROR("Unable to load file.");
    return false;
  }
  return true;
}

bool ConfigManager::save(std::string_view filepath) {
  std::string output;
  return createXmlDocument(true, filepath, output);
}

void ConfigManager::setWarnOnEmptyKey(bool warnOnEmptyKey) const {
  m_warnOnEmptyKey = warnOnEmptyKey;
}

bool ConfigManager::createXmlDocument(bool saveAsFile, std::string_view filepath, std::string& output) {
  bool success = true;
  TiXmlDocument doc;
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  auto* decl = new TiXmlDeclaration("1.0", "utf-8", "");
  doc.LinkEndChild(decl);
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  auto* root = new TiXmlElement("config");
  doc.LinkEndChild(root);

  for(auto& m_value : m_values) {
    if(m_value.first.empty() || m_value.second.empty()) {
      continue;
    }

    std::vector<std::string> tokens = utility::splitToVector(m_value.first, "/");

    TiXmlElement* element = doc.RootElement();
    TiXmlElement* child = nullptr;
    while(tokens.size() > 1) {
      child = element->FirstChildElement(tokens.front().c_str());
      if(child == nullptr) {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        child = new TiXmlElement(tokens.front().c_str());
        element->LinkEndChild(child);
      }
      tokens.erase(tokens.begin());
      element = child;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    child = new TiXmlElement(tokens.front().c_str());
    element->LinkEndChild(child);
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    auto* text = new TiXmlText(m_value.second.c_str());
    child->LinkEndChild(text);
  }

  if(saveAsFile) {
    success = doc.SaveFile(filepath.data());
  } else {
    TiXmlPrinter printer;
    doc.Accept(&printer);
    output = printer.CStr();
  }
  success = doc.SaveFile(filepath.data());
  doc.Clear();
  return success;
}

void ConfigManager::parseSubtree(TiXmlNode* currentNode, const std::string& currentPath) {
  if(currentNode->Type() == TiXmlNode::TINYXML_TEXT) {
    std::string key = currentPath.substr(0, currentPath.size() - 1);
    m_values.insert(std::pair<std::string, std::string>(key, currentNode->ToText()->Value()));
  } else {
    for(TiXmlNode* childNode = currentNode->FirstChild(); childNode != nullptr; childNode = childNode->NextSibling()) {
      parseSubtree(childNode, currentPath + std::string(currentNode->Value()) + "/");
    }
  }
}

std::string ConfigManager::toString() {
  std::string output;
  createXmlDocument(false, "", output);
  return output;
}