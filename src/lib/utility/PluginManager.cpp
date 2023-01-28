#include "PluginManager.h"
// STL
#include <filesystem>
// internal
#include "ApplicationSettings.h"
#include "logging.h"
// Qt5
#include <QTimer>

namespace fs = std::filesystem;

PluginManager::PluginManager(QObject* pParent) noexcept : QObject {pParent} {}

PluginManager::~PluginManager() = default;

void PluginManager::loadPlugins() {
  auto pluginsDir = ApplicationSettings::getInstance()->getPluginDir();
  if(!(fs::exists(pluginsDir) && fs::is_directory(pluginsDir))) {
    LOG_WARNING("Plugins dir is not exists");
    return;
  }

  // Call later
  QTimer::singleShot(0, [this, pluginsDir = std::move(pluginsDir)]() {
    for(auto file : fs::directory_iterator {pluginsDir}) {
      if(file.is_regular_file()) {
      }
    }
  });
}

void PluginManager::loadPlugin(std::string_view plugName) {}