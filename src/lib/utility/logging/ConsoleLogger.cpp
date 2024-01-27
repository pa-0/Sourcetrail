#include "ConsoleLogger.h"
// STL
#include <fmt/core.h>
#include <fmt/printf.h>
// Internal
#include "LogMessage.h"
#include "utilityString.h"

constexpr auto ASCII_BLUE = "\u001b[34m";
constexpr auto ASCII_YELLOW = "\u001b[33m";
constexpr auto ASCII_RED = "\u001b[31m";
constexpr auto ASCII_RESET = "\u001b[0m";

ConsoleLogger::ConsoleLogger() noexcept : Logger("ConsoleLogger") {}

ConsoleLogger::~ConsoleLogger() = default;

void ConsoleLogger::logInfo(const LogMessage& message) {
  logMessage(std::string {ASCII_BLUE} + "INFO" + ASCII_RESET, message);
}

void ConsoleLogger::logWarning(const LogMessage& message) {
  logMessage(std::string {ASCII_YELLOW} + "WARNING" + ASCII_RESET, message);
}

void ConsoleLogger::logError(const LogMessage& message) {
  logMessage(std::string {ASCII_RED} + "ERROR" + ASCII_RESET, message);
}

void ConsoleLogger::logMessage(const std::string& type, const LogMessage& message) {
  fmt::print("{} | ", message.getTimeString());

  if(!message.filePath.empty()) {
    fmt::print("{}:{} {}() | ", message.getFileName(), message.line, message.functionName);
  }

  fmt::print("{}: {}\n", type, utility::encodeToUtf8(message.message));
}