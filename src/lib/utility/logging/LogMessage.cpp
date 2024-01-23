#include "LogMessage.h"

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <utility>

LogMessage::LogMessage(std::wstring message,
                       std::string filePath,
                       std::string functionName,
                       const unsigned int line,
                       const std::time_t& time,
                       const std::thread::id& threadId)
    : message(std::move(message))
    , filePath(std::move(filePath))
    , functionName(std::move(functionName))
    , line(line)
    , time(time)
    , threadId(threadId) {}

std::string LogMessage::getTimeString() const {
  return fmt::format("{:%H:%M:%S}", fmt::localtime(time));
}

std::string LogMessage::getFileName() const {
  return filePath.substr(filePath.find_last_of("/\\") + 1);
}