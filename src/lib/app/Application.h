#pragma once
// STL
#include <memory>
// internal
#include "DialogView.h"
#include "MessageActivateWindow.h"
#include "MessageCloseProject.h"
#include "MessageIndexingFinished.h"
#include "MessageListener.h"
#include "MessageLoadProject.h"
#include "MessageRefresh.h"
#include "MessageRefreshUI.h"
#include "MessageSwitchColorScheme.h"
#include "Project.h"

class Bookmark;
class IDECommunicationController;
class MainView;
class NetworkFactory;
class StorageCache;
class UpdateChecker;
class Version;
class ViewFactory;

class Application
    : public MessageListener<MessageActivateWindow>
    , public MessageListener<MessageCloseProject>
    , public MessageListener<MessageIndexingFinished>
    , public MessageListener<MessageLoadProject>
    , public MessageListener<MessageRefresh>
    , public MessageListener<MessageRefreshUI>
    , public MessageListener<MessageSwitchColorScheme> {
public:
  static void createInstance(const Version& version,
                             ViewFactory* viewFactory,
                             NetworkFactory* networkFactory);
  static std::shared_ptr<Application> getInstance();
  static void destroyInstance();

  static std::string getUUID();

  static void loadSettings();
  static void loadStyle(const FilePath& colorSchemePath);

  ~Application() override;

  /**
   * @name Disable copy and move operators
   * @{ */
  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;
  /**  @} */

  [[nodiscard]] std::shared_ptr<const Project> getCurrentProject() const;
  [[nodiscard]] FilePath getCurrentProjectPath() const;
  [[nodiscard]] bool isProjectLoaded() const;

  [[nodiscard]] bool hasGUI() const;

  int handleDialog(const std::wstring& message);
  int handleDialog(const std::wstring& message, const std::vector<std::wstring>& options);
  std::shared_ptr<DialogView> getDialogView(DialogView::UseCase useCase);

  void updateHistoryMenu(std::shared_ptr<MessageBase> message);
  void updateBookmarks(const std::vector<std::shared_ptr<Bookmark>>& bookmarks);

private:
  static std::shared_ptr<Application> s_instance;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  static std::string s_uuid;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  explicit Application(bool withGUI = true);

  /**
   * @name handle Message group
   * @{ */
  void handleMessage(MessageActivateWindow* pMessage) override;
  void handleMessage(MessageCloseProject* pMessage) override;
  void handleMessage(MessageIndexingFinished* pMessage) override;
  /**
   * @brief Handle a load project message
   *
   * @param pMessage a message
   */
  void handleMessage(MessageLoadProject* pMessage) override;
  void handleMessage(MessageRefresh* pMessage) override;
  void handleMessage(MessageRefreshUI* pMessage) override;
  void handleMessage(MessageSwitchColorScheme* pMessage) override;
  /**  @} */

  void startMessagingAndScheduling();

  void loadWindow(bool showStartWindow);

  void refreshProject(RefreshMode refreshMode, bool shallowIndexingRequested);
  void updateRecentProjects(const FilePath& projectSettingsFilePath);

  void logStorageStats() const;

  void updateTitle();

  bool checkSharedMemory();

  const bool m_hasGUI;
  bool m_loadedWindow = false;

  std::shared_ptr<Project> m_pProject;
  std::shared_ptr<StorageCache> m_storageCache;

  std::shared_ptr<MainView> m_mainView;

  std::shared_ptr<IDECommunicationController> m_ideCommunicationController;
};