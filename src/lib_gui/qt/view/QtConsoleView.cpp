#include "QtConsoleView.hpp"
// STL
#include <memory>
// fmt
#include <fmt/core.h>
// Qt5
#include <QFrame>
#include <QLineEdit>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
// internal
#include "ConsoleController.hpp"
#include "QtViewWidgetWrapper.h"

QtConsoleView::QtConsoleView(ViewLayout* pViewLayout)
    : ConsoleView(pViewLayout), m_pLineEdit {new QLineEdit}, m_pEditor {new QTextEdit} {
  // Must be called
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): Qt manage the memory
  setWidgetWrapper(std::make_shared<QtViewWidgetWrapper>(new QFrame));

  auto* pWidget = QtViewWidgetWrapper::getWidgetOfView(this);

  auto* pLayout = new QVBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory): Qt manage the memory
  pLayout->setContentsMargins(0, 0, 0, 0);
  pLayout->setSpacing(0);
  pLayout->addWidget(m_pEditor);
  pLayout->addWidget(m_pLineEdit);
  pWidget->setLayout(pLayout);

  m_pEditor->setReadOnly(true);

  // format-off
  connect(m_pLineEdit, &QLineEdit::returnPressed,
          this, &QtConsoleView::returnButtonPressed);

  // NOTE(Hussein): get ConsoleController after the constructor scope
  QTimer::singleShot(0, [this]() {
    m_pController = getController<ConsoleController>();
  });
  // format-on
}

QtConsoleView::~QtConsoleView() = default;

void QtConsoleView::createWidgetWrapper() {}

void QtConsoleView::refreshView() {}

void QtConsoleView::returnButtonPressed() {
  auto text = m_pLineEdit->text().toStdString();
  m_pLineEdit->clear();
  if(text.empty()) {
    m_pEditor->append(">");
    return;
  }

  if(m_pController != nullptr) {
    if(const auto result = m_pController->process(text); !result.empty()) {
      m_pEditor->append(QString::fromStdString(fmt::format("> {}\n{}", text, result)));
      return;
    }
  }

  m_pEditor->append(QString::fromStdString(fmt::format("> {}", text)));
}