#pragma once
// Qt5
#include <QWidget>
// internal
#include "ConsoleView.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QTextEdit)

class ConsoleController;

class QtConsoleView final
    : public QWidget
    , public ConsoleView {
public:
  explicit QtConsoleView(ViewLayout* pViewLayout);

  Q_DISABLE_COPY_MOVE(QtConsoleView)

  ~QtConsoleView() override;

  // View implementation
  void createWidgetWrapper() override;

  void refreshView() override;

private slots:
  void returnButtonPressed();

private:
  ConsoleController* m_pController = nullptr;
  QLineEdit *m_pLineEdit;
  QTextEdit *m_pEditor;
};