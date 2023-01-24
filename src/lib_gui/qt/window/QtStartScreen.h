#pragma once
// Qt5
#include <QPushButton>
// Internal
#include "FilePath.h"
#include "QtWindow.h"

class QtRecentProjectButton : public QPushButton {
  Q_OBJECT

public:
  static QtRecentProjectButton* create(QWidget* pParent);

  explicit QtRecentProjectButton(QWidget* parent);

  bool projectExists() const;

  void setProjectPath(const FilePath& projectFilePath);

public slots:
  void handleButtonClick();

signals:
  void updateButtons();

protected:
  void contextMenuEvent(QContextMenuEvent* pEvent) override;

private:
  bool m_projectExists;
  FilePath m_projectFilePath;
};

class QtStartScreen : public QtWindow {
  Q_OBJECT

public:
  QtStartScreen(QWidget* parent = 0);
  QSize sizeHint() const override;

  void setupStartScreen();

signals:
  void openOpenProjectDialog();
  void openNewProjectDialog();

private slots:
  void handleNewProjectButton();
  void handleOpenProjectButton();
  void handleRecentButton();
  void updateButtons();

private:
  std::vector<QtRecentProjectButton*> m_recentProjectsButtons;
};