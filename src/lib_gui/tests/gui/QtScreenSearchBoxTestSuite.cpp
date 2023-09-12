#include "QtScreenSearchBoxTestSuite.hpp"

#include <QHBoxLayout>
#include <QLineEdit>

#include "MockedView.hpp"
#include "QtScreenSearchBox.h"
#include "ScreenSearchController.h"
#include "ControllerProxy.h"
#include "MockedViewLayout.hpp"
#include "QtSelfRefreshIconButton.h"

void QtScreenSearchBoxTest::testingStructure() {
  MockedViewLayout viewLayout;
  MockedView view(&viewLayout);
  ControllerProxy<ScreenSearchController> controllerProxy(&view, TabId::app());
  const QtScreenSearchBox searchBox(&controllerProxy);

  auto* childLayout = searchBox.findChild<QHBoxLayout*>();
  QVERIFY2(childLayout != nullptr, "A QHBoxLayout will be created");
  QVERIFY2(searchBox.layout() == childLayout, "QHBoxLayout is the widget layout");

  auto buttons = searchBox.findChildren<QtSelfRefreshIconButton*>();
  ASSERT_EQ(buttons.size(), 5);

  std::set<std::string> buttonsNames = {
    "search_button", "prev_button", "next_button", "close_button", "match_label"
  };
  for(auto* button : buttons) {
    auto buttonName = button->objectName().toStdString();
    auto found = buttonsNames.find(buttonName);
    EXPECT_NE(found, buttonsNames.end()) << "Button name \"" << buttonName << "\" is not found in the list";
    QVERIFY2(childLayout->indexOf(button) != -1, "QLineEdit is member of the layout");
  }

  auto layouts = searchBox.findChildren<QHBoxLayout*>();
  ASSERT_EQ(layouts.size(), 2);

  auto* searchEdit = searchBox.findChild<QLineEdit*>();
  QVERIFY2(searchEdit != nullptr, "QLineEdit is created");
  QVERIFY2(childLayout->indexOf(searchEdit) != -1, "QLineEdit is member of the layout");

  // TODO: Check install event filter
}

QTEST_MAIN(QtScreenSearchBoxTest)
