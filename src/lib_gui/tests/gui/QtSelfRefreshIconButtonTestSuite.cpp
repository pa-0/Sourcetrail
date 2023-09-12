#include "QtSelfRefreshIconButtonTestSuite.hpp"

#include "QtSelfRefreshIconButton.h"
#include "ResourcePaths.h"

void QtSelfRefreshIconButtonTest::testingConstructWithEmptyString() {
  QtSelfRefreshIconButton button({}, FilePath {}, {});

  QResizeEvent resizeEvent(QSize(800, 600), QSize(400, 300));
  QCoreApplication::sendEvent(&button, &resizeEvent);
  QTest::qSleep(50);

  QCOMPARE(std::string {""}, button.text().toStdString());
}

void QtSelfRefreshIconButtonTest::testingConstruct() {
  QtSelfRefreshIconButton button("button", FilePath {}, {});

  QResizeEvent resizeEvent(QSize(800, 600), QSize(400, 300));
  QCoreApplication::sendEvent(&button, &resizeEvent);
  QTest::qSleep(50);

  QCOMPARE(std::string {"button"}, button.text().toStdString());
}

void QtSelfRefreshIconButtonTest::testingSetText() {
  QtSelfRefreshIconButton button({}, FilePath {}, {});
  button.setText("Hello");
  QCOMPARE(std::string {"Hello"}, button.text().toStdString());

  button.setAutoElide(true);
  button.setText("HelloWorld11111111111111111111111111111111111111111111111111");

  QResizeEvent resizeEvent(QSize(100, 50), button.baseSize());
  QCoreApplication::sendEvent(&button, &resizeEvent);
  QTest::qSleep(50);

  QCOMPARE(std::string {"…111111"}, button.text().toStdString());
}

void QtSelfRefreshIconButtonTest::testingSetTextWithEmptyString() {
  QtSelfRefreshIconButton button({}, FilePath {}, {});
  button.setText({});
  QCOMPARE(std::string {""}, button.text().toStdString());
}

void QtSelfRefreshIconButtonTest::testingSetIconPath() {
  QtSelfRefreshIconButton button({}, FilePath {}, "search/button");
  // HACK
  auto iconPath = FilePath{"../app"}.concatenate(ResourcePaths::getGuiDirectoryPath().concatenate(L"graph_view/images/graph_custom.png"));
  button.setIconPath(iconPath);
  QVERIFY(!button.icon().isNull());
}

void QtSelfRefreshIconButtonTest::testingSetIconPathWithEmptyString() {
  QtSelfRefreshIconButton button({}, FilePath {}, "search/button");
  button.setIconPath({});
  QVERIFY(button.icon().isNull());
}

QTEST_MAIN(QtSelfRefreshIconButtonTest)