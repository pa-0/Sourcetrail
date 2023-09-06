#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "IndexerCommand.h"
#include "IndexerComposite.h"
#include "IntermediateStorage.h"
#include "LogManager.h"
#include "MockedIndexer.hpp"
#include "MockedLogger.hpp"

using namespace testing;

struct MockedIndexerCommand final : public IndexerCommand {
  MockedIndexerCommand() : IndexerCommand(FilePath {}) {}
  MOCK_METHOD(IndexerCommandType, getIndexerCommandType, (), (const, override));
};

struct IndexerCompositeFix : testing::Test {
  void SetUp() override {
    mLogManager->setLoggingEnabled(true);

    mMockedLogger = std::make_shared<MockedLogger>();
    mMockedLogger->setLogLevel(Logger::LogLevel::LOG_ALL);
    mLogManager->addLogger(mMockedLogger);
  }
  LogManager* mLogManager = LogManager::getInstance().get();
  std::shared_ptr<MockedLogger> mMockedLogger;
};

TEST(IndexerComposite, getSupportedIndexerCommandType) {
  EXPECT_EQ(INDEXER_COMMAND_UNKNOWN, IndexerComposite{}.getSupportedIndexerCommandType());
}

TEST_F(IndexerCompositeFix, missingIndexer) {
  IndexerComposite indexerComposite;
  EXPECT_CALL(*mMockedLogger, logError(_)).WillOnce(Return());

  auto mockedIndexerCommand = std::make_shared<MockedIndexerCommand>();
  EXPECT_CALL(*mockedIndexerCommand, getIndexerCommandType)
    .WillOnce(Return(INDEXER_COMMAND_CXX))
    .WillOnce(Return(INDEXER_COMMAND_CXX));
  EXPECT_FALSE(indexerComposite.index(mockedIndexerCommand));
}

TEST(IndexerComposite, goodCase) {
  IndexerComposite indexerComposite;

  auto mockedIndexerCommand = std::make_shared<MockedIndexerCommand>();
  EXPECT_CALL(*mockedIndexerCommand, getIndexerCommandType)
    .WillOnce(Return(INDEXER_COMMAND_CXX));
  auto mockedIndexer = std::make_shared<MockedIndexer>();
  EXPECT_CALL(*mockedIndexer, getSupportedIndexerCommandType).WillOnce(Return(INDEXER_COMMAND_CXX));
  indexerComposite.addIndexer(mockedIndexer);

  auto intermediateStorage = std::make_shared<IntermediateStorage>();
  EXPECT_CALL(*mockedIndexer, index).WillOnce(Return(intermediateStorage));
  auto result = indexerComposite.index(mockedIndexerCommand);

  EXPECT_CALL(*mockedIndexer, interrupt).WillOnce(Return());
  indexerComposite.interrupt();
}