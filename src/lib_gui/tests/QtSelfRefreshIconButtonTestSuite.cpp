#include <QApplication>
#include <QTest>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "QtSelfRefreshIconButton.h"
#include "ResourcePaths.h"

using namespace testing;

struct QtScreenSearchBoxFix : testing::Test {};

TEST_F(QtScreenSearchBoxFix, testingConstructWithEmptyString) {
  QtSelfRefreshIconButton button({}, FilePath {}, {});

  QResizeEvent resizeEvent(QSize(800, 600), QSize(400, 300));
  QCoreApplication::sendEvent(&button, &resizeEvent);
  QTest::qSleep(50);

  EXPECT_THAT(button.text().toStdString(), IsEmpty());
}

TEST_F(QtScreenSearchBoxFix, testingConstruct) {
  QtSelfRefreshIconButton button("button", FilePath {}, {});

  QResizeEvent resizeEvent(QSize(800, 600), QSize(400, 300));
  QCoreApplication::sendEvent(&button, &resizeEvent);
  QTest::qSleep(50);

  EXPECT_THAT(button.text().toStdString(), StrEq("button"));
}

TEST_F(QtScreenSearchBoxFix, testingSetText) {
  QtSelfRefreshIconButton button({}, FilePath {}, {});
  button.setText("Hello");
  QCOMPARE(std::string {"Hello"}, button.text().toStdString());

  button.setAutoElide(true);
  button.setText("HelloWorld11111111111111111111111111111111111111111111111111");

  QResizeEvent resizeEvent(QSize(100, 50), button.baseSize());
  QCoreApplication::sendEvent(&button, &resizeEvent);
  QTest::qSleep(50);

  EXPECT_THAT(button.text().toStdString(), StrEq("…111111"));
}

TEST_F(QtScreenSearchBoxFix, testingSetTextWithEmptyString) {
  QtSelfRefreshIconButton button({}, FilePath {}, {});
  button.setText({});
  EXPECT_THAT(button.text().toStdString(), IsEmpty());
}

TEST_F(QtScreenSearchBoxFix, testingSetIconPath) {
  QtSelfRefreshIconButton button({}, FilePath {}, "search/button");
  // HACK
  auto iconPath = FilePath{"../app"}.concatenate(ResourcePaths::getGuiDirectoryPath().concatenate(L"graph_view/images/graph_custom.png"));
  button.setIconPath(iconPath);
  EXPECT_FALSE(button.icon().isNull());
}

TEST_F(QtScreenSearchBoxFix, testingSetIconPathWithEmptyString) {
  QtSelfRefreshIconButton button({}, FilePath {}, "search/button");
  button.setIconPath({});
  EXPECT_TRUE(button.icon().isNull());
}

int main(int argc, char* argv[]) {
  const QApplication application {argc, argv};

  QTimer::singleShot(0, &application, [&]() {
    testing::InitGoogleTest(&argc, argv);
    const auto code = RUN_ALL_TESTS();
    QApplication::exit(code);
  });

  return QApplication::exec();
}