#include "ConsoleView.hpp"
// internal
#include "TabId.h"

ConsoleView::ConsoleView(ViewLayout* pViewLayout)
    : View(pViewLayout), m_pControllerProxy(this, TabId::app()) {}

ConsoleView::~ConsoleView() = default;

std::string ConsoleView::getName() const {
  return "ConsoleView";
}