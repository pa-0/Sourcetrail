#pragma once

#include <QMenu>
#include <QObject>

#include "FilePath.h"

class QAction;
class QWidget;

class QtContextMenu final : public QObject {
  Q_OBJECT

public:
  QtContextMenu(const QContextMenuEvent* event, QWidget* origin);

	Q_DISABLE_COPY_MOVE(QtContextMenu)

	~QtContextMenu() override;

  void addAction(QAction* action);

  void addUndoActions();

  void addFileActions(const FilePath& filePath);

  static QtContextMenu* getInstance();

  void addSeparator();

  void show();

  void enableUndo(bool enabled);

  void enableRedo(bool enabled);

private:
	QtContextMenu();

  // slots:
  void undoActionTriggered();

  void redoActionTriggered();

  void copyFullPathActionTriggered();

  void openContainingFolderActionTriggered();

  static QtContextMenu* s_instance;

  static QAction* s_undoAction;
  static QAction* s_redoAction;

  static QAction* s_copyFullPathAction;
  static QAction* s_openContainingFolderAction;

  static FilePath s_filePath;

  QMenu m_menu;
  QPoint m_point;
};