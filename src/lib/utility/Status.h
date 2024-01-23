#pragma once
// STL
#include <string>
#include <utility>

enum StatusType {
  STATUS_INFO = 1,
  STATUS_ERROR = 2,
};

using StatusFilter = int;

struct Status {
  Status(std::wstring message_, bool isError_ = false)
      : message(std::move(message_)), type(isError_ ? StatusType::STATUS_ERROR : StatusType::STATUS_INFO) {}

  std::wstring message;
  StatusType type;
};