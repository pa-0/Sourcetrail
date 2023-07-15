#include "ScopedTemporaryFile.hpp"

#include <filesystem>
#include <fstream>
#include <memory>

#include <fmt/format.h>

#include <system_error>

#include "logging.h"

namespace fs = std::filesystem;

namespace utility {

ScopedTemporaryFile::Ptr ScopedTemporaryFile::createEmptyFile(std::filesystem::path filePath) {
  std::error_code errorCode;
  if(!filePath.empty() && fs::exists(filePath, errorCode)) {
  }
  auto handler = ScopedTemporaryFile::Ptr(new ScopedTemporaryFile);
  handler->mFilePath = std::move(filePath);
  return handler;
}

ScopedTemporaryFile::Ptr ScopedTemporaryFile::createFile(std::filesystem::path filePath, std::string_view data) {
  auto handler = createEmptyFile(std::move(filePath));
  if(!handler) {
    return {};
  }

  std::ofstream oStream(handler->mFilePath);
  if(!oStream.is_open()) {
    LOG_WARNING(fmt::format("Can not open \"{}\" file", handler->mFilePath.string()));
    return {};
  }
  oStream.write(data.data(), static_cast<long>(data.size()));
  oStream.close();

  if(fs::file_size(handler->mFilePath) != data.size()) {
    LOG_WARNING("The written file doesn't match the buffer");
    return {};
  }

  return handler;
}

ScopedTemporaryFile::ScopedTemporaryFile() = default;

ScopedTemporaryFile::~ScopedTemporaryFile() {
  std::error_code errorCode;
  if(!mFilePath.empty() && fs::exists(mFilePath, errorCode)) {
    if(!fs::remove(mFilePath, errorCode)) {
      LOG_WARNING(fmt::format("Can not remove \"{}\" reason \"{}\"", mFilePath.string(), errorCode.message()));
    }
    mFilePath.clear();
  }
}


}    // namespace utility