#pragma once
// STL
#include <string>

enum StatusType {
  STATUS_INFO = 1,
  STATUS_ERROR = 2,
};

using StatusFilter = int;

struct Status {
  Status(std::wstring message, bool isError = false)
      : message(message), type(isError ? StatusType::STATUS_ERROR : StatusType::STATUS_INFO) {}

  std::wstring message;
  StatusType type;
};