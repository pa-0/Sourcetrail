#pragma once
// STL
#include <string>
#include <memory>
#include <unordered_map>
// Boost
#include <boost/dll/import.hpp>
// Qt5
#include <QObject>

class IPlugin;
using IPluginPtr = std::shared_ptr<IPlugin>;

class PluginManager final : public QObject {
  Q_OBJECT
public:
  explicit PluginManager(QObject* pParent = nullptr) noexcept;

  ~PluginManager() override;

  void loadPlugins();

  void loadPlugin(std::string_view plugName);

private:
  std::unordered_map<std::string, IPluginPtr> mDynamicLibraries;
};