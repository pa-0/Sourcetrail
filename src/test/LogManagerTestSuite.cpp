#include <thread>

#include <gtest/gtest.h>

#include "LogManagerImplementation.h"
#include "Logger.h"

namespace {
class TestLogger : public Logger {
public:
  TestLogger();

  void reset();
  int getMessageCount() const;
  int getWarningCount() const;
  int getErrorCount() const;

  std::wstring getLastInfo() const;
  std::wstring getLastWarning() const;
  std::wstring getLastError() const;

private:
  void logInfo(const LogMessage& message) override;
  void logWarning(const LogMessage& message) override;
  void logError(const LogMessage& message) override;

  int m_logMessageCount;
  int m_logWarningCount;
  int m_logErrorCount;

  std::wstring m_lastInfo;
  std::wstring m_lastWarning;
  std::wstring m_lastError;
};

TestLogger::TestLogger()
    : Logger("TestLogger")
    , m_logMessageCount(0)
    , m_logWarningCount(0)
    , m_logErrorCount(0)
    , m_lastInfo(L"")
    , m_lastWarning(L"")
    , m_lastError(L"") {}

void TestLogger::reset() {
  m_logMessageCount = 0;
  m_logWarningCount = 0;
  m_logErrorCount = 0;
}

int TestLogger::getMessageCount() const {
  return m_logMessageCount;
}

int TestLogger::getWarningCount() const {
  return m_logWarningCount;
}

int TestLogger::getErrorCount() const {
  return m_logErrorCount;
}

std::wstring TestLogger::getLastInfo() const {
  return m_lastInfo;
}

std::wstring TestLogger::getLastWarning() const {
  return m_lastWarning;
}

std::wstring TestLogger::getLastError() const {
  return m_lastError;
}

void TestLogger::logInfo(const LogMessage& message) {
  m_lastInfo = message.message;
  m_logMessageCount++;
}

void TestLogger::logWarning(const LogMessage& message) {
  m_lastWarning = message.message;
  m_logWarningCount++;
}

void TestLogger::logError(const LogMessage& message) {
  m_lastError = message.message;
  m_logErrorCount++;
}


void addTestLogger(LogManagerImplementation* logManagerImplementation, const unsigned int loggerCount) {
  for(unsigned int i = 0; i < loggerCount; i++) {
    std::shared_ptr<Logger> logger = std::make_shared<TestLogger>();
    logManagerImplementation->addLogger(logger);
  }
}

void removeTestLoggers(LogManagerImplementation* logManagerImplementation) {
  std::shared_ptr<Logger> logger = std::make_shared<TestLogger>();
  logManagerImplementation->removeLoggersByType(logger->getType());
}

void addAndRemoveTestLogger(LogManagerImplementation* logManagerImplementation, const unsigned int loggerCount) {
  addTestLogger(logManagerImplementation, loggerCount);
  removeTestLoggers(logManagerImplementation);
}

void logSomeMessages(LogManagerImplementation* logManagerImplementation,
                     const std::wstring& message,
                     const unsigned int messageCount) {
  for(unsigned int i = 0; i < messageCount; i++) {
    logManagerImplementation->logInfo(message, __FILE__, __FUNCTION__, __LINE__);
    logManagerImplementation->logWarning(message, __FILE__, __FUNCTION__, __LINE__);
    logManagerImplementation->logError(message, __FILE__, __FUNCTION__, __LINE__);
  }
}
}    // namespace

TEST(LogManagerTestSuite, newLoggerCanBeAddedToManager) {
  LogManagerImplementation logManagerImplementation;

  std::shared_ptr<Logger> logger = std::make_shared<TestLogger>();

  int countBeforeAdd = logManagerImplementation.getLoggerCount();
  logManagerImplementation.addLogger(logger);
  int countAfterAdd = logManagerImplementation.getLoggerCount();
  logManagerImplementation.removeLogger(logger);

  EXPECT_TRUE(1 == countAfterAdd - countBeforeAdd);
}

TEST(LogManagerTestSuite, loggerCanBeRemovedFromManager) {
  LogManagerImplementation logManagerImplementation;

  std::shared_ptr<Logger> logger = std::make_shared<TestLogger>();

  int countBeforeAdd = logManagerImplementation.getLoggerCount();
  logManagerImplementation.addLogger(logger);
  logManagerImplementation.removeLogger(logger);
  int countAfterRemove = logManagerImplementation.getLoggerCount();

  EXPECT_TRUE(countBeforeAdd == countAfterRemove);
}

TEST(LogManagerTestSuite, loggerLogsMessage) {
  LogManagerImplementation logManagerImplementation;

  const std::wstring log = L"test";
  std::shared_ptr<TestLogger> logger = std::make_shared<TestLogger>();

  logManagerImplementation.addLogger(logger);
  logManagerImplementation.logInfo(log, __FILE__, __FUNCTION__, __LINE__);
  const int logCount = logger->getMessageCount();
  const std::wstring lastLog = logger->getLastInfo();

  EXPECT_TRUE(1 == logCount);
  EXPECT_TRUE(log == lastLog);
}

TEST(LogManagerTestSuite, loggerLogsWarning) {
  LogManagerImplementation logManagerImplementation;

  const std::wstring log = L"test";
  std::shared_ptr<TestLogger> logger = std::make_shared<TestLogger>();

  logManagerImplementation.addLogger(logger);

  logManagerImplementation.logWarning(log, __FILE__, __FUNCTION__, __LINE__);
  const int logCount = logger->getWarningCount();
  const std::wstring lastLog = logger->getLastWarning();

  EXPECT_TRUE(1 == logCount);
  EXPECT_TRUE(log == lastLog);
}

TEST(LogManagerTestSuite, loggerLogsError) {
  LogManagerImplementation logManagerImplementation;

  std::wstring log = L"test";
  std::shared_ptr<TestLogger> logger = std::make_shared<TestLogger>();

  logManagerImplementation.addLogger(logger);

  logManagerImplementation.logError(log, __FILE__, __FUNCTION__, __LINE__);
  const int logCount = logger->getErrorCount();
  const std::wstring lastLog = logger->getLastError();

  EXPECT_TRUE(1 == logCount);
  EXPECT_TRUE(log == lastLog);
}

TEST(LogManagerTestSuite, loggerLogsOnlyLogsOfDefinedLogLevel) {
  LogManagerImplementation logManagerImplementation;

  std::wstring info = L"info";
  std::wstring warning = L"warning";
  std::wstring error = L"error";

  std::shared_ptr<TestLogger> logger = std::make_shared<TestLogger>();

  logger->setLogLevel(Logger::LOG_INFOS | Logger::LOG_ERRORS);

  logManagerImplementation.addLogger(logger);

  logManagerImplementation.logInfo(info, __FILE__, __FUNCTION__, __LINE__);
  logManagerImplementation.logWarning(warning, __FILE__, __FUNCTION__, __LINE__);
  logManagerImplementation.logError(error, __FILE__, __FUNCTION__, __LINE__);

  EXPECT_TRUE(1 == logger->getMessageCount());
  EXPECT_TRUE(0 == logger->getWarningCount());
  EXPECT_TRUE(1 == logger->getErrorCount());

  EXPECT_TRUE(info == logger->getLastInfo());
  EXPECT_TRUE(error == logger->getLastError());
}

TEST(LogManagerTestSuite, newLoggerCanBeAddedToManagerThreaded) {
  LogManagerImplementation logManagerImplementation;
  unsigned int loggerCount = 100;

  std::thread thread0(addTestLogger, &logManagerImplementation, loggerCount);
  std::thread thread1(addTestLogger, &logManagerImplementation, loggerCount);

  thread0.join();
  thread1.join();

  EXPECT_TRUE(loggerCount * 2 == logManagerImplementation.getLoggerCount());
}

TEST(LogManagerTestSuite, loggerCanBeRemovedFromManagerThreaded) {
  LogManagerImplementation logManagerImplementation;
  unsigned int loggerCount = 100;

  std::thread thread0(addAndRemoveTestLogger, &logManagerImplementation, loggerCount);
  std::thread thread1(addAndRemoveTestLogger, &logManagerImplementation, loggerCount);

  thread0.join();
  thread1.join();

  EXPECT_TRUE(0 == logManagerImplementation.getLoggerCount());
}

TEST(LogManagerTestSuite, loggerLogsThreaded) {
  LogManagerImplementation logManagerImplementation;

  std::wstring log = L"foo";
  unsigned int messageCount = 100;
  std::shared_ptr<TestLogger> logger = std::make_shared<TestLogger>();
  logManagerImplementation.addLogger(logger);

  std::thread thread0(logSomeMessages, &logManagerImplementation, log, messageCount);
  std::thread thread1(logSomeMessages, &logManagerImplementation, log, messageCount);

  thread0.join();
  thread1.join();

  EXPECT_TRUE(logger->getLastError() == log);
  EXPECT_TRUE(messageCount * 6 == logger->getErrorCount() + logger->getWarningCount() + logger->getMessageCount());
}
