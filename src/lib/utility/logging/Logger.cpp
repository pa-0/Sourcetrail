#include "Logger.h"

Logger::LogLevel Logger::convertStringToLogLevel(const std::string& level) {
  if(level == "INFOS") {
    return LogLevel::LOG_INFOS;
  }

  if(level == "WARNINGS") {
    return LogLevel::LOG_WARNINGS;
  }

  if(level == "ERRORS") {
    return LogLevel::LOG_ERRORS;
  }
  return LogLevel::LOG_ALL;
}

Logger::Logger(std::string type) : m_type(std::move(type)), m_levelMask(LOG_ALL) {}

Logger::~Logger() = default;

std::string Logger::getType() const {
  return m_type;
}

Logger::LogLevelMask Logger::getLogLevel() const {
  return m_levelMask;
}

void Logger::setLogLevel(LogLevelMask mask) {
  m_levelMask = mask;
}

bool Logger::isLogLevel(LogLevelMask mask) const {
  return (m_levelMask & mask) > 0;    // NOLINT(hicpp-signed-bitwise)
}

void Logger::onInfo(const LogMessage& message) {
  if(isLogLevel(LOG_INFOS)) {
    logInfo(message);
  }
}

void Logger::onWarning(const LogMessage& message) {
  if(isLogLevel(LOG_WARNINGS)) {
    logWarning(message);
  }
}

void Logger::onError(const LogMessage& message) {
  if(isLogLevel(LOG_ERRORS)) {
    logError(message);
  }
}
