#include "QtContextMenuTestSuite.hpp"

#include <set>
#include <string>

#include <QAction>
#include <QTest>

#include "QtContextMenu.h"
#include "utilities.hpp"

void QtContextMenuTest::testingStructure() {
  auto* instance = QtContextMenu::getInstance();

  auto actions = instance->findChildren<QAction*>();
  QCOMPARE(4, actions.size());

  const std::set<std::string> actionNames = {"Back", "Forward", "Copy Full Path", "Open Containing Folder"};
  QVERIFY(std::none_of(std::cbegin(actions), std::cend(actions), [&actionNames](const QAction* action) {
    const auto actionText = action->text().toStdString();
    qDebug() << actionText.c_str();
    return actionNames.find(actionText) == actionNames.cend();
  }));
}

void QtContextMenuTest::testingAddUndoActions() {
  QContextMenuEvent event(QContextMenuEvent::Reason::Mouse, QPoint {});
  QWidget widget;
  QtContextMenu contextMenu(&event, &widget);

  contextMenu.addUndoActions();
}

void QtContextMenuTest::testingAddileActions() {
  auto* instance = QtContextMenu::getInstance();
  instance->addFileActions(FilePath{"/tmp"});

  auto* action = utilities::getChildByText<QAction>(*instance, "Copy Full Path");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());

  action = utilities::getChildByText<QAction>(*instance, "Open Containing Folder");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());
}

void QtContextMenuTest::testingEnableUndo() {
  auto* instance = QtContextMenu::getInstance();
  auto* action = utilities::getChildByText<QAction>(*instance, "Back");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());
  instance->enableUndo(false);
  QVERIFY(!action->isEnabled());
}

void QtContextMenuTest::testingEnableRedo() {
  auto* instance = QtContextMenu::getInstance();
  auto* action = utilities::getChildByText<QAction>(*instance, "Forward");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());
  instance->enableRedo(false);
  QVERIFY(!action->isEnabled());
}

QTEST_MAIN(QtContextMenuTest)
