#include "ConsoleView.hpp"

ConsoleView::ConsoleView(ViewLayout* pViewLayout) : View(pViewLayout) {}

ConsoleView::~ConsoleView() = default;

std::string ConsoleView::getName() const {
    return "ConsoleView";
}