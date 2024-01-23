#pragma once
// STL
#include <vector>
#include <string>
// internal
#include "LogMessage.h"

class Logger {
public:
  using LogLevelMask = int;
  enum LogLevel : int { LOG_INFOS = 0x1, LOG_WARNINGS = 0x2, LOG_ERRORS = 0x4, LOG_ALL = 0x7 };

  static LogLevel convertStringToLogLevel(const std::string& level);

  explicit Logger(std::string type);

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  virtual ~Logger();

  [[nodiscard]] std::string getType() const;

  [[nodiscard]] LogLevelMask getLogLevel() const;

  void setLogLevel(LogLevelMask mask);

  [[nodiscard]] bool isLogLevel(LogLevelMask mask) const;

  void onInfo(const LogMessage& message);

  void onWarning(const LogMessage& message);

  void onError(const LogMessage& message);

private:
  virtual void logInfo(const LogMessage& message) = 0;

  virtual void logWarning(const LogMessage& message) = 0;

  virtual void logError(const LogMessage& message) = 0;

  const std::string m_type;
  LogLevelMask m_levelMask;
};
