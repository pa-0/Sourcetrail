#pragma once
#include <QObject>

struct QtContextMenuTest final : QObject {
  Q_OBJECT
private slots:
  void testingStructure();
  void testingAddUndoActions();
  void testingAddileActions();
  void testingEnableUndo();
  void testingEnableRedo();
};