#include "QtScreenSearchBoxTestSuite.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSignalSpy>

#include "QtScreenSearchBox.h"
#include "QtSelfRefreshIconButton.h"
#include "utilities.hpp"

void QtScreenSearchBoxTest::init() {
  viewLayout = std::make_unique<MockedViewLayout>();
  view = std::make_unique<MockedView>(viewLayout.get());
  controllerProxy = std::make_unique<ControllerProxy<ScreenSearchController>>(view.get(), TabId::app());
  searchBox = std::make_unique<QtScreenSearchBox>(controllerProxy.get());
}

void QtScreenSearchBoxTest::testingStructure() {
  auto* childLayout = searchBox->findChild<QHBoxLayout*>();
  QVERIFY2(childLayout != nullptr, "A QHBoxLayout will be created");
  QVERIFY2(searchBox->layout() == childLayout, "QHBoxLayout is the widget layout");

  auto buttons = searchBox->findChildren<QtSelfRefreshIconButton*>();
  ASSERT_EQ(buttons.size(), 5);

  std::set<std::string> buttonsNames = {"search_button", "prev_button", "next_button", "close_button", "match_label"};
  for(auto* button : buttons) {
    auto buttonName = button->objectName().toStdString();
    auto found = buttonsNames.find(buttonName);
    EXPECT_NE(found, buttonsNames.end()) << "Button name \"" << buttonName << "\" is not found in the list";
    QVERIFY2(childLayout->indexOf(button) != -1, "QLineEdit is member of the layout");
  }

  auto layouts = searchBox->findChildren<QHBoxLayout*>();
  ASSERT_EQ(layouts.size(), 2);

  auto* searchEdit = searchBox->findChild<QLineEdit*>();
  QVERIFY2(searchEdit != nullptr, "QLineEdit is created");
  QVERIFY2(childLayout->indexOf(searchEdit) != -1, "QLineEdit is member of the layout");

  // TODO: Check install event filter
}

void QtScreenSearchBoxTest::testingSetMatchCountZero() {
  searchBox->setMatchCount(0);
  const std::set<std::string> buttonsNames = {"prev_button", "next_button"};
  auto buttons = searchBox->findChildren<QtSelfRefreshIconButton*>();
  for(auto* button : buttons) {
    auto buttonName = button->objectName().toStdString();
    auto found = buttonsNames.find(buttonName);
    if(found == buttonsNames.end()) {
      continue;
    }
    QVERIFY(button->isEnabled() == false);
  }
  const auto* matchButton = utilities::getChildByObjectName<QPushButton>(*searchBox, "match_label");
  QVERIFY(matchButton != nullptr);
  ASSERT_EQ(matchButton->text(), "0 matches");
}

void QtScreenSearchBoxTest::testingSetMatchCount() {
  searchBox->setMatchCount(2);
  const std::set<std::string> buttonsNames = {"prev_button", "next_button"};
  auto buttons = searchBox->findChildren<QtSelfRefreshIconButton*>();
  for(auto* button : buttons) {
    auto buttonName = button->objectName().toStdString();
    auto found = buttonsNames.find(buttonName);
    if(found == buttonsNames.end()) {
      continue;
    }
    QVERIFY(button->isEnabled());
  }
  const auto* matchButton = utilities::getChildByObjectName<QPushButton>(*searchBox, "match_label");
  QVERIFY(matchButton != nullptr);
  ASSERT_EQ(matchButton->text(), "2 matches");
}

void QtScreenSearchBoxTest::testingSetMatchIndexZero() {
  searchBox->setMatchIndex(0);
  const auto* matchButton = utilities::getChildByObjectName<QPushButton>(*searchBox, "match_label");
  QVERIFY(matchButton != nullptr);
  ASSERT_EQ(matchButton->text(), "0 matches");
}

void QtScreenSearchBoxTest::testingSetMatchIndex() {
  searchBox->setMatchIndex(2);
  const auto* matchButton = utilities::getChildByObjectName<QPushButton>(*searchBox, "match_label");
  QVERIFY(matchButton != nullptr);
  ASSERT_EQ(matchButton->text(), "2 of 0 matches");
}

void QtScreenSearchBoxTest::testingAddResponderEmpty() {
  searchBox->addResponder({});
  auto buttons = searchBox->findChildren<QCheckBox*>();
  ASSERT_EQ(0, buttons.size());
}

void QtScreenSearchBoxTest::testingAddResponderExists() {
  searchBox->addResponder("code");
  auto buttons = searchBox->findChildren<QCheckBox*>();
  ASSERT_EQ(1, buttons.size());
  searchBox->addResponder("code");
  buttons = searchBox->findChildren<QCheckBox*>();
  ASSERT_EQ(1, buttons.size());
}

void QtScreenSearchBoxTest::testingCloseSignal() {
  auto* closeButton = utilities::getChildByObjectName<QtSelfRefreshIconButton>(*searchBox, "close_button");
  QVERIFY(closeButton != nullptr);
  QSignalSpy spy(closeButton, &QPushButton::clicked);
  QTest::mouseClick(closeButton, Qt::MouseButton::LeftButton);
  QCOMPARE(1, spy.count());
}

void QtScreenSearchBoxTest::testingTextChangedSignal() {
  auto* searchButton = utilities::getChildByObjectName<QLineEdit>(*searchBox, "search_box");
  QVERIFY(searchButton != nullptr);
  QTest::keyClicks(searchButton, "hello world");
  // TODO: missing checks
}

void QtScreenSearchBoxTest::testingReturnPressedSignal() {
  auto* searchButton = utilities::getChildByObjectName<QLineEdit>(*searchBox, "search_box");
  QVERIFY(searchButton != nullptr);
  QTest::keyClick(searchButton, Qt::Key_Enter);
  QTest::keyClick(searchButton, Qt::Key_Enter, Qt::ShiftModifier);
  // TODO: missing checks
}

void QtScreenSearchBoxTest::cleanup() {
  viewLayout.reset();
  view.reset();
  controllerProxy.reset();
  searchBox.reset();
}

QTEST_MAIN(QtScreenSearchBoxTest)
