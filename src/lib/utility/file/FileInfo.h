#pragma once
// internal
#include "FilePath.h"
#include "TimeStamp.h"

struct FileInfo final {
  FileInfo();
  FileInfo(const FilePath& path);
  FileInfo(const FilePath& path, const TimeStamp& lastWriteTime);

  FilePath path;
  TimeStamp lastWriteTime;
};