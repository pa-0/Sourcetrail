#pragma once
// Qt5
#include <QWidget>
// internal
#include "ConsoleView.hpp"

QT_FORWARD_DECLARE_CLASS(QTextEdit)

class QtConsoleView final
    : public QWidget
    , public ConsoleView {
public:
  explicit QtConsoleView(ViewLayout* pViewLayout);

  ~QtConsoleView() override;

  // View implementation
  void createWidgetWrapper() override;

  void refreshView() override;

private:
  void setupViewWidgetWrapper();

private slots:
  void returnButtonPressed();

private:
  QTextEdit *m_pEditor;
};