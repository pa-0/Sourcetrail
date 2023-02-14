#include "QtCoreApplication.h"
// STL
#include <iostream>

QtCoreApplication::QtCoreApplication(int argc, char** argv) : QCoreApplication(argc, argv) {}

QtCoreApplication::~QtCoreApplication() = default;

void QtCoreApplication::handleMessage([[maybe_unused]] MessageQuitApplication* pMessage) {
  std::cout << "Quitting" << std::endl;
  emit quit();
}

void QtCoreApplication::handleMessage(MessageIndexingStatus* pMessage) {
  if(pMessage->showProgress) {
    std::cout << pMessage->progressPercent << "% " << '\r' << std::flush;
  }
}

void QtCoreApplication::handleMessage(MessageStatus* pMessage) {
  if(pMessage->isError) {
    std::wcout << L"ERROR: ";
  }

  for(const std::wstring& status : pMessage->stati()) {
    std::wcout << status << std::endl;
  }
}
