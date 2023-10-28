#pragma once
#include <memory>
#include <string>
#include <vector>

#include "RefreshInfo.h"
#include "SourceGroup.h"

class DialogView;
class FilePath;
class PersistentStorage;
class ProjectSettings;
class StorageCache;
struct FileInfo;
struct ProjectBuilderIndex;

#ifdef ST_TESTING
#include <gtest/gtest.h>
struct ProjectFix;
#endif

/**
 * @brief Project object
 */
class Project final {
public:
  /**
   * @brief Constructor
   *
   * @param settings Project settings
   * @param storageCache Storage Cache
   * @param hasGUI The application started with GUI
   */
  Project(std::shared_ptr<ProjectSettings> settings, StorageCache* storageCache, std::string appUUID, bool hasGUI);
  /**
   * @name Disable copy and move operators
   * @{ */
  Project(const Project&) = delete;
  Project(Project&&) = delete;
  Project& operator=(const Project&) = delete;
  Project& operator=(Project&&) = delete;
  /**  @} */

  ~Project();

  [[nodiscard]] FilePath getProjectSettingsFilePath() const;
  [[nodiscard]] std::string getDescription() const;

  [[nodiscard]] bool isLoaded() const;
  [[nodiscard]] bool isIndexing() const;

  [[nodiscard]] bool settingsEqualExceptNameAndLocation(const ProjectSettings& otherSettings) const;
  void setStateOutdated();

  void load(const std::shared_ptr<DialogView>& dialogView);

  void refresh(std::shared_ptr<DialogView> dialogView, RefreshMode refreshMode, bool shallowIndexingRequested);

  [[nodiscard]] RefreshInfo getRefreshInfo(RefreshMode mode) const;

  void buildIndex(RefreshInfo info, std::shared_ptr<DialogView> dialogView);

private:
  enum class ProjectStateType { NOT_LOADED, EMPTY, LOADED, OUTDATED, OUTVERSIONED, NEEDS_MIGRATION, DB_CORRUPTED };

  enum class RefreshStageType { REFRESHING, INDEXING, NONE };

  void swapToTempStorage(std::shared_ptr<DialogView> dialogView);
  bool swapToTempStorageFile(const FilePath& indexDbFilePath,
                             const FilePath& tempIndexDbFilePath,
                             std::shared_ptr<DialogView> dialogView);
  void discardTempStorage();

  [[nodiscard]] bool hasCxxSourceGroup() const;

  std::shared_ptr<ProjectSettings> m_settings;
  StorageCache* m_storageCache;

  ProjectStateType m_state;
  RefreshStageType m_refreshStage;

  std::shared_ptr<PersistentStorage> m_storage;
  std::vector<std::shared_ptr<SourceGroup>> m_sourceGroups;

  std::string m_appUUID;
  bool m_hasGUI;

  friend ProjectBuilderIndex;
#ifdef ST_TESTING
  FRIEND_TEST(ProjectFix, loadWhileIndexing);
  FRIEND_TEST(ProjectFix, loadFailed);
#endif
};
