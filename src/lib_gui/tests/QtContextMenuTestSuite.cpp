#include <set>
#include <string>

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QTimer>

#include <gtest/gtest.h>

#include "QtContextMenu.h"
#include "gui/utilities.hpp"

struct QtContextMenuFix : testing::Test {};

TEST_F(QtContextMenuFix, testingStructure) {
  const auto* instance = QtContextMenu::getInstance();

  auto actions = instance->findChildren<QAction*>();
  ASSERT_EQ(4, actions.size());

  const auto actionNames = std::set<std::string> {"Back", "Forward", "Copy Full Path", "Open Containing Folder"};

  EXPECT_TRUE(std::none_of(std::cbegin(actions), std::cend(actions), [&actionNames](const QAction* action) {
    const auto actionText = action->text().toStdString();
    return actionNames.find(actionText) == actionNames.cend();
  }));
}

TEST_F(QtContextMenuFix, testingEnableUndo) {
  auto* instance = QtContextMenu::getInstance();

  auto* action = utilities::getChildByText<QAction>(*instance, "Back");
  ASSERT_NE(nullptr, action);
  EXPECT_TRUE(action->isEnabled());

  instance->enableUndo(false);
  EXPECT_FALSE(action->isEnabled());
}

TEST_F(QtContextMenuFix, testingEnableRedo) {
  auto* instance = QtContextMenu::getInstance();

  auto* action = utilities::getChildByText<QAction>(*instance, "Forward");
  ASSERT_NE(nullptr, action);
  EXPECT_TRUE(action->isEnabled());

  instance->enableRedo(false);
  EXPECT_FALSE(action->isEnabled());
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
