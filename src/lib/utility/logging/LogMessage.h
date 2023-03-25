#pragma once
// STL
#include <chrono>
#include <string>
#include <thread>

struct LogMessage final {
  LogMessage(const std::wstring& message,
             const std::string& filePath,
             const std::string& functionName,
             const uint32_t line,
             const std::time_t& time,
             const std::thread::id& threadId);

	[[nodiscard]] std::string getTimeString(const std::string& format) const;

  [[nodiscard]] std::string getFileName() const;

  const std::wstring message;
  const std::string filePath;
  const std::string functionName;
  const uint32_t line;
  const std::time_t time;
  const std::thread::id threadId;
};