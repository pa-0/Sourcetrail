#include "QtStartScreenTestSuite.hpp"

#include <memory>

#include <QTest>

#include "QtStartScreen.hpp"

QtStartScreenTestSuite::QtStartScreenTestSuite() = default;

QtStartScreenTestSuite::~QtStartScreenTestSuite() = default;

void QtStartScreenTestSuite::init() {
  mScreen = std::make_unique<qt::window::QtStartScreen>();
  mScreen->show();
  QVERIFY(QTest::qWaitForWindowActive(mScreen.get()));
}

void QtStartScreenTestSuite::goodCase() {
  QVERIFY(mScreen->isSubWindow());
  QCOMPARE(QSize(600, 650), mScreen->sizeHint());
}

QTEST_MAIN(QtStartScreenTestSuite)