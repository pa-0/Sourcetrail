#pragma once
// STL
#include <vector>
// internal
#include "ErrorInfo.h"

struct ErrorCountInfo {
  ErrorCountInfo() = default;

  ErrorCountInfo(size_t total_, size_t fatal_) : total(total_), fatal(fatal_) {}

  ErrorCountInfo(const std::vector<ErrorInfo>& errors) {
    for(const ErrorInfo& error : errors) {
      total++;

      if(error.fatal) {
        fatal++;
      }
    }
  }

  size_t total = 0;
  size_t fatal = 0;
};