#include "LogMessage.h"
// fmt
#include <fmt/chrono.h>

LogMessage::LogMessage(const std::wstring& message,
                       const std::string& filePath,
                       const std::string& functionName,
                       const unsigned int line,
                       const std::time_t& time,
                       const std::thread::id& threadId)
    : message(message)
    , filePath(filePath)
    , functionName(functionName)
    , line(line)
    , time(time)
    , threadId(threadId) {}

std::string LogMessage::getTimeString(const std::string& format) const {
  return fmt::format(format, fmt::localtime(time));
}

std::string LogMessage::getFileName() const {
  return filePath.substr(filePath.find_last_of("/\\") + 1);
}