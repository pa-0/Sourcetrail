#pragma once
// internal
#include "Logger.h"

struct LogMessage;

class ConsoleLogger final : public Logger {
public:
  ConsoleLogger() noexcept;

	~ConsoleLogger() override;

private:
  void logInfo(const LogMessage& message) override;
  void logWarning(const LogMessage& message) override;
  void logError(const LogMessage& message) override;

  void logMessage(const std::string& type, const LogMessage& message);
};