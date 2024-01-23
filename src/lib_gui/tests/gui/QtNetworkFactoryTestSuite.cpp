#include "QtNetworkFactoryTestSuite.hpp"

#include <QTest>

#include "QtNetworkFactory.h"

void QtNetworkFactoryTestSuite::goodCase() {
  const QtNetworkFactory factory;
  QVERIFY(factory.createIDECommunicationController(nullptr) != nullptr);
}

QTEST_MAIN(QtNetworkFactoryTestSuite)