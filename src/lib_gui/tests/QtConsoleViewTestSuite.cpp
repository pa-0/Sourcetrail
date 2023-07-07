// STL
#include <chrono>
#include <thread>
// catch2
#include <catch2/catch_all.hpp>
// trompeloeil
#include <trompeloeil.hpp>
// Qt5
#include <QApplication>
#include <QLineEdit>
#include <QTextEdit>
#include <QTimer>
// internal
#include "LogManager.h"
#include "Logger.h"
#include "QtConsoleView.hpp"
#include "QtViewWidgetWrapper.h"

using namespace std::chrono_literals;

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
class MockLogger final : public Logger {
public:
  MockLogger() : Logger("Mocked") {}
  ~MockLogger() override = default;

  MAKE_MOCK1(logInfo, void(const LogMessage&));       // NOLINT(readability-identifier-length)
  MAKE_MOCK1(logWarning, void(const LogMessage&));    // NOLINT(readability-identifier-length)
  MAKE_MOCK1(logError, void(const LogMessage&));      // NOLINT(readability-identifier-length)
};

class MockViewLayout : public trompeloeil::mock_interface<ViewLayout> {
  MAKE_MOCK1(addView, void(View*));                 // NOLINT(readability-identifier-length)
  MAKE_MOCK1(overrideView, void(View*));            // NOLINT(readability-identifier-length)
  MAKE_MOCK1(removeView, void(View*));              // NOLINT(readability-identifier-length)
  MAKE_MOCK1(showView, void(View*));                // NOLINT(readability-identifier-length)
  MAKE_MOCK1(hideView, void(View*));                // NOLINT(readability-identifier-length)
  MAKE_MOCK2(setViewEnabled, void(View*, bool));    // NOLINT(readability-identifier-length)
};

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("structure", "[lib_gui]") {
  MockViewLayout mockViewLayout;

  QtConsoleView view(&mockViewLayout);

  // Make sure that the singleShot is called
  QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

  const auto* pParent = QtViewWidgetWrapper::getWidgetOfView(&view);

  // Check QTextEdit
  auto* pTextEdit = pParent->findChild<QTextEdit*>();
  REQUIRE_FALSE(pTextEdit == nullptr);
  REQUIRE(pTextEdit->isReadOnly());

  // Check QLineEdit
  auto* pLineEdit = pParent->findChild<QLineEdit*>();
  REQUIRE_FALSE(pLineEdit == nullptr);
  // TODO(Hussein): Check return signal is connect to returnButtonPressed
}

// TODO(Hussein): Move to anther file
int main(int argc, char** argv) {
  QApplication app(argc, argv);

  auto* pLogManager = LogManager::getInstance().get();

  auto pLogger = std::make_shared<MockLogger>();
  pLogger->setLogLevel(Logger::LOG_ALL);
  pLogManager->addLogger(pLogger);


  return app.exec();
}