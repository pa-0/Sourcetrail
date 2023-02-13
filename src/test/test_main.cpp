#define CATCH_CONFIG_MAIN    // This tells Catch to provide a main() function
// STL
#include <iostream>
// catch2
#include <catch2/catch_all.hpp>
// IMPORTANT NOTE: removed signal listener for "EXCEPTION_ACCESS_VIOLATION" from catch source code
// because it interferes with the jni interface that emits such a signal on purpose

#include "ApplicationSettings.h"
#include "language_packages.h"
#include "utilityPathDetection.h"

struct EventListener : Catch::EventListenerBase {
  using EventListenerBase::EventListenerBase;    // inherit constructor

  void testRunStarting(const Catch::TestRunInfo& testRunInfo) override {
#ifdef __linux__
    const std::string homedir = getenv("HOME");

    if(!homedir.empty()) {
      if(!ApplicationSettings::getInstance()->load(
             FilePath(homedir + "/.config/sourcetrail/ApplicationSettings.xml"))) {
        std::cout << "no settings" << std::endl;
        return;
      }
    } else {
      std::cout << "no homedir" << std::endl;
      return;
    }
#else
    ApplicationSettings::getInstance()->load(FilePath(L"data/TestSettings.xml"));
#endif
  }
};

CATCH_REGISTER_LISTENER(EventListener)
