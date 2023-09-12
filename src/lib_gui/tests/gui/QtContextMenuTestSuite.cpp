#include "QtContextMenuTestSuite.hpp"

#include <set>
#include <string>

#include <QAction>
#include <QTest>

#include "QtContextMenu.h"

namespace {
template <class Type>
Type* getChildByText(const QtContextMenu& object, const QString& name) {
  auto types = object.findChildren<Type*>();
  if(types.empty()) {
    return nullptr;
  }
  if(types.size() == 1) {
    auto* type = types.front();
    return (type->text() == name) ? type : nullptr;
  }
  auto itr = std::find_if(std::cbegin(types), std::cend(types), [&name](auto* type) {
    return type->text() == name;
  });
  return (itr == std::cend(types)) ? nullptr : *itr;
}
}    // namespace

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

  auto* action = getChildByText<QAction>(*instance, "Copy Full Path");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());

  action = getChildByText<QAction>(*instance, "Open Containing Folder");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());
}

void QtContextMenuTest::testingEnableUndo() {
  auto* instance = QtContextMenu::getInstance();
  auto* action = getChildByText<QAction>(*instance, "Back");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());
  instance->enableUndo(false);
  QVERIFY(!action->isEnabled());
}

void QtContextMenuTest::testingEnableRedo() {
  auto* instance = QtContextMenu::getInstance();
  auto* action = getChildByText<QAction>(*instance, "Forward");
  QVERIFY(action != nullptr);
  QVERIFY(action->isEnabled());
  instance->enableRedo(false);
  QVERIFY(!action->isEnabled());
}

QTEST_MAIN(QtContextMenuTest)