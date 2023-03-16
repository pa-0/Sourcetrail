#pragma once
// STL
#include <memory>
#include <string>
#include <vector>
// internal
#include "RefreshInfo.h"
#include "SourceGroup.h"

struct FileInfo;
class DialogView;
class FilePath;
class PersistentStorage;
class ProjectSettings;
class StorageCache;

class Project final {
public:
  Project(std::shared_ptr<ProjectSettings> settings,
          StorageCache* storageCache,
          std::string appUUID,
          bool hasGUI);

  Project(const Project&) = delete;
	Project(Project&&) = delete;
	Project& operator=(const Project&) = delete;
	Project& operator=(Project&&) = delete;

  ~Project();

  [[nodiscard]] FilePath getProjectSettingsFilePath() const;
  [[nodiscard]] std::string getDescription() const;

  [[nodiscard]] bool isLoaded() const;
  [[nodiscard]] bool isIndexing() const;

  [[nodiscard]] bool settingsEqualExceptNameAndLocation(const ProjectSettings& otherSettings) const;
  void setStateOutdated();

  void load(std::shared_ptr<DialogView> dialogView);

  void refresh(std::shared_ptr<DialogView> dialogView,
               RefreshMode refreshMode,
               bool shallowIndexingRequested);

  [[nodiscard]] RefreshInfo getRefreshInfo(RefreshMode mode) const;

  void buildIndex(RefreshInfo info, std::shared_ptr<DialogView> dialogView);

private:
  enum ProjectStateType {
    PROJECT_STATE_NOT_LOADED,
    PROJECT_STATE_EMPTY,
    PROJECT_STATE_LOADED,
    PROJECT_STATE_OUTDATED,
    PROJECT_STATE_OUTVERSIONED,
    PROJECT_STATE_NEEDS_MIGRATION,
    PROJECT_STATE_DB_CORRUPTED
  };

  enum class RefreshStageType { REFRESHING, INDEXING, NONE };

  void swapToTempStorage(std::shared_ptr<DialogView> dialogView);
  bool swapToTempStorageFile(const FilePath& indexDbFilePath,
                             const FilePath& tempIndexDbFilePath,
                             std::shared_ptr<DialogView> dialogView);
  void discardTempStorage();

  [[nodiscard]] bool hasCxxSourceGroup() const;

  std::shared_ptr<ProjectSettings> m_settings;
  StorageCache* const m_storageCache;

  ProjectStateType m_state;
  RefreshStageType m_refreshStage;

  std::shared_ptr<PersistentStorage> m_storage;
  std::vector<std::shared_ptr<SourceGroup>> m_sourceGroups;

  std::string m_appUUID;
  bool m_hasGUI;
};