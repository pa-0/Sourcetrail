#include <type_traits>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "AppPath.h"
#include "ApplicationSettings.h"
#include "InterprocessIndexer.h"
#include "LanguagePackageManager.h"
#include "includes.h"
#include "language_packages.h"
#include "logging.h"

#if BUILD_CXX_LANGUAGE_PACKAGE
#  include "LanguagePackageCxx.h"
#endif    // BUILD_CXX_LANGUAGE_PACKAGE

void setupLogging(const std::string& logFilePath) {
  auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath);
  fileSink->set_level(spdlog::level::trace);
  spdlog::set_default_logger(std::make_shared<spdlog::logger>("indexer", fileSink));
}

void suppressCrashMessage() {
#ifdef _WIN32
  SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
#endif    // _WIN32
}

int main(int argc, char* argv[]) {
  int processId = -1;
  std::string instanceUuid;
  std::string appPath;
  std::string userDataPath;
  std::string logFilePath;

  if(argc >= 2) {
    processId = std::stoi(argv[1]);
  }

  if(argc >= 3) {
    instanceUuid = argv[2];
  }

  if(argc >= 4) {
    appPath = argv[3];
  }

  if(argc >= 5) {
    userDataPath = argv[4];
  }

  if(argc >= 6) {
    logFilePath = argv[5];
  }

  AppPath::setSharedDataDirectoryPath(FilePath(appPath));
  UserPaths::setUserDataDirectoryPath(FilePath(userDataPath));

  if(!logFilePath.empty()) {
    setupLogging(std::string(logFilePath));
  }

  suppressCrashMessage();

  ApplicationSettings* appSettings = ApplicationSettings::getInstance().get();
  appSettings->load(UserPaths::getAppSettingsFilePath());

  LOG_INFO_W(L"sharedDataPath: " + AppPath::getSharedDataDirectoryPath().wstr());
  LOG_INFO_W(L"userDataPath: " + UserPaths::getUserDataDirectoryPath().wstr());


#if BUILD_CXX_LANGUAGE_PACKAGE
  LanguagePackageManager::getInstance()->addPackage(std::make_shared<LanguagePackageCxx>());
#endif    // BUILD_CXX_LANGUAGE_PACKAGE

  InterprocessIndexer indexer(instanceUuid, Id(processId));
  indexer.work();

  return 0;
}