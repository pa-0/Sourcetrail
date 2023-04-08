#include "QtConsoleView.hpp"
// STL
#include <memory>
// Qt5
#include <QFrame>
#include <QLayout>
#include <QTextEdit>
// internal
#include "QtViewWidgetWrapper.h"

QtConsoleView::QtConsoleView(ViewLayout* pViewLayout)
    : ConsoleView(pViewLayout), m_pEditor {new QTextEdit} {
  layout()->addWidget(m_pEditor);
  // Must be called
  setWidgetWrapper(std::make_shared<QtViewWidgetWrapper>(new QFrame));

  auto* pWidget = QtViewWidgetWrapper::getWidgetOfView(this);

  auto* pLayout = new QVBoxLayout;
  pLayout->setContentsMargins(0, 0, 0, 0);
  pLayout->setSpacing(0);
  pWidget->setLayout(pLayout);

  pLayout->addWidget(m_pEditor);
}

QtConsoleView::~QtConsoleView() = default;

void QtConsoleView::createWidgetWrapper() {}

void QtConsoleView::refreshView() {}

void QtConsoleView::returnButtonPressed() {}