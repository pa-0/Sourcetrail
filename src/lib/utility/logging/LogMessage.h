#pragma once
// STL
#include <chrono>
#include <string>
#include <thread>

struct LogMessage final {
  LogMessage(std::wstring message,
             std::string filePath,
             std::string functionName,
             const uint32_t line,
             const std::time_t& time,
             const std::thread::id& threadId);

  [[nodiscard]] std::string getTimeString() const;

  [[nodiscard]] std::string getFileName() const;

  const std::wstring message;
  const std::string filePath;
  const std::string functionName;
  const uint32_t line;
  const std::time_t time;
  const std::thread::id threadId;
};