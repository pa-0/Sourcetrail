// GTest
#include <gmock/gmock.h>
#include <gtest/gtest.h>
// internal
#include "Logger.h"
#include "MockedLogger.hpp"

using namespace ::testing;
using namespace std::string_literals;

// NOLINTNEXTLINE
TEST(Logger, DISABLED_defaultState) {
  MockedLogger logger;
  EXPECT_THAT(logger.getType(), StrEq("MockedLogger"));
  EXPECT_EQ(0x0001, logger.getLogLevel());
  EXPECT_TRUE(logger.isLogLevel(0x0001));
}
