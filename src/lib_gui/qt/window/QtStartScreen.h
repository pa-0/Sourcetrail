#pragma once
// Internal
#include "QtWindow.h"

class QtRecentProjectButton;

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
  void updateButtons();

private:
  std::vector<QtRecentProjectButton*> m_recentProjectsButtons;
};