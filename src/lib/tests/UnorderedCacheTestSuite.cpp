// GTest
#include <gmock/gmock.h>
#include <gtest/gtest.h>
// internal
#include "UnorderedCache.h"

using namespace ::testing;

// NOLINTNEXTLINE
TEST(UnorderedCache, goodCase) {
  uint32_t count = 0;
  UnorderedCache<int, int> cache([&count](int) {
    ++count;
    return 0;
  });
  EXPECT_EQ(0, cache.getValue(0));
  EXPECT_EQ(1, count);
  EXPECT_EQ(0, cache.getValue(0));
  EXPECT_EQ(1, count);
}