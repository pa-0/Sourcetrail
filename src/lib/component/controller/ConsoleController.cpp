#include "ConsoleController.hpp"
// internal
#include "ConsoleInterpreter.hpp"

ConsoleController::ConsoleController(std::unique_ptr<IConsoleInterpreter> pParser)
    : m_pParser {std::move(pParser)} {}

ConsoleController::~ConsoleController() = default;

std::string ConsoleController::process(const std::string&  text) {
  return m_pParser->process(text);
}

void ConsoleController::clear() {}