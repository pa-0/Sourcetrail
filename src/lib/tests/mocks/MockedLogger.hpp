#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Logger.h"

struct LogMessage;

class MockedLogger final : public Logger {
public:
  MockedLogger() : Logger("MockedLogger") {}

  MOCK_METHOD(void, logInfo, (const LogMessage&), (override));

  MOCK_METHOD(void, logWarning, (const LogMessage&), (override));

  MOCK_METHOD(void, logError, (const LogMessage&), (override));
};