#include "ConsoleController.hpp"
// internal
#include "ConsoleParser.hpp"
// internal
#include "StorageAccess.h"

ConsoleController::ConsoleController([[maybe_unused]] StorageAccess* pStorageAccess)
    : m_pParser {std::make_unique<ConsoleParser>(pStorageAccess)} {}

ConsoleController::~ConsoleController() = default;

std::string ConsoleController::process(const std::string& text) {
    return m_pParser->process(text);
}

void ConsoleController::clear() {}