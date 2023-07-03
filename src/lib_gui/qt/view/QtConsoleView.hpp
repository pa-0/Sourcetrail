#pragma once
// Qt5
#include <QObject>
// internal
#include "ConsoleView.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QTextEdit)

class ConsoleController;

class QtConsoleView final
    : public QObject,
      public ConsoleView {
public:
  explicit QtConsoleView(ViewLayout* pViewLayout);

  ~QtConsoleView() override;

  // View implementation
  void createWidgetWrapper() override;

  void refreshView() override;

private slots:
  void returnButtonPressed();

private: // NOLINT(readability-redundant-access-specifiers)
  ConsoleController* m_pController = nullptr;
  QLineEdit *m_pLineEdit;
  QTextEdit *m_pEditor;
};