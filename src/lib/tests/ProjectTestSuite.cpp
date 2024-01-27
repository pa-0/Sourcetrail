#include <chrono>
#include <memory>
#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ConsoleLogger.h"
#include "FilePath.h"
#include "MessageListener.h"
#include "MessageStatus.h"
#include "Project.h"
#include "mocks/MockedDialogView.hpp"
#include "mocks/MockedLogger.hpp"
#include "mocks/MockedProjectSettings.hpp"
#include "mocks/MockedStorageCache.hpp"

using namespace std::chrono_literals;
using namespace testing;

constexpr auto WaitingDuration = 100ms;

struct MockedMessageStatus : MessageListener<MessageStatus> {
  MOCK_METHOD(void, handleMessage, (MessageStatus*), (override));
};

struct ProjectFix : Test {
  void setupLogger() {
    auto* logManager = LogManager::getInstance().get();
    logManager->setLoggingEnabled(true);

    mLogger = std::make_shared<StrictMock<MockedLogger>>();
    mLogger->setLogLevel(Logger::LogLevel::LOG_ALL);
    logManager->addLogger(mLogger);
  }

  void SetUp() override {
    setupLogger();
    MessageQueue::getInstance()->startMessageLoopThreaded();

    mSettings = std::make_shared<StrictMock<MockedProjectSettings>>();
    mStorageCache = std::make_unique<StrictMock<MockedStorageCache>>();
    mProject = std::make_unique<Project>(mSettings, mStorageCache.get(), "", true);

    mDialogView = std::make_shared<StrictMock<MockedDialogView>>();
  }

  void TearDown() override {
    MessageQueue::getInstance()->stopMessageLoop();
    LogManager::getInstance()->removeLogger(mLogger);
  }

  std::shared_ptr<StrictMock<MockedLogger>> mLogger;
  std::shared_ptr<StrictMock<MockedProjectSettings>> mSettings;
  std::unique_ptr<StrictMock<MockedStorageCache>> mStorageCache;
  std::unique_ptr<Project> mProject;
  std::shared_ptr<StrictMock<MockedDialogView>> mDialogView;
  MockedMessageStatus mStatus;
};

TEST_F(ProjectFix, loadWhileIndexing) {
  EXPECT_CALL(*mLogger, logInfo(_)).WillOnce(Return());
  EXPECT_CALL(mStatus, handleMessage(_)).WillOnce(Return());

  mProject->m_refreshStage = Project::RefreshStageType::INDEXING;
  mProject->load(mDialogView);

  std::this_thread::sleep_for(WaitingDuration);
}

TEST_F(ProjectFix, loadFailedToReloadSettings) {
  EXPECT_CALL(*mStorageCache, setUseErrorCache(_)).WillOnce(Return());
  EXPECT_CALL(*mSettings, load(_, _)).WillOnce(Return(false));

  mProject->load(mDialogView);

  std::this_thread::sleep_for(WaitingDuration);
}

TEST_F(ProjectFix, loadGoodCase) {
  EXPECT_CALL(*mSettings, load(_, _)).WillOnce(Return(true));
  EXPECT_CALL(*mStorageCache, setUseErrorCache(_)).WillOnce(Return());
  EXPECT_CALL(*mLogger, logInfo(_)).Times(2);

  EXPECT_CALL(mStatus, handleMessage(_)).WillOnce(Return());

  mProject->load(mDialogView);

  std::this_thread::sleep_for(WaitingDuration);

  EXPECT_TRUE(mProject->isLoaded());
  EXPECT_FALSE(mProject->isIndexing());
}

TEST_F(ProjectFix, DISABLED_loadFailed) {
  EXPECT_CALL(*mSettings, load(_, _)).WillOnce(Return(true));
  EXPECT_CALL(*mStorageCache, setUseErrorCache(_)).WillOnce(Return());

  // mProject->m_refreshStage = Project::RefreshStageType::NONE;
  mProject->load(mDialogView);

  EXPECT_TRUE(mProject->isLoaded());
  EXPECT_FALSE(mProject->isIndexing());
}

TEST_F(ProjectFix, DISABLED_buildIndex) {
  EXPECT_CALL(*mDialogView, clearDialogs()).WillOnce(Return());

  const RefreshInfo info {{FilePath {"1.cpp"}}, {}, {}, REFRESH_ALL_FILES};
  mProject->buildIndex(info, mDialogView);
}
