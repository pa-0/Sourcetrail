// GTest
#include <gmock/gmock.h>
#include <gtest/gtest.h>
// internal
#include "Logger.h"

using namespace ::testing;
using namespace std::string_literals;

class MockLogger final : public Logger {
public:
  MockLogger() : Logger("MockLogger") {}

private:
  MOCK_METHOD(void, logInfo, (const LogMessage&), (override));

  MOCK_METHOD(void, logWarning, (const LogMessage&), (override));

  MOCK_METHOD(void, logError, (const LogMessage&), (override));
};

// NOLINTNEXTLINE
TEST(Logger, DISABLED_defaultState) {
  MockLogger logger;
  EXPECT_THAT(logger.getType(), StrEq("MockLogger"));
  EXPECT_EQ(0x0001, logger.getLogLevel());
  EXPECT_TRUE(logger.isLogLevel(0x0001));
}
