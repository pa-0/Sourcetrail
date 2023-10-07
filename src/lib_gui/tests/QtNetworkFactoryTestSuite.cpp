// gtest
#include <gtest/gtest.h>
// internal
#include "QtNetworkFactory.h"

TEST(QtNetworkFactory, goodCase) {
  const QtNetworkFactory factory;
  EXPECT_NE(nullptr, factory.createIDECommunicationController(nullptr));
}