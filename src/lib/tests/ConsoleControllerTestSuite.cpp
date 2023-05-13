// catch2
#include <catch2/catch_all.hpp>
// trompeloeil
#include <memory>
#include <trompeloeil.hpp>
// internal
#include "ConsoleController.hpp"
#include "ConsoleInterpreter.hpp"
#include "mocks/MockStorageAccess.hpp"

using namespace Catch::Matchers;

struct MockedConsoleInterpreter : IConsoleInterpreter {
  MAKE_MOCK1(process, std::string(const std::string&)); // NOLINT(readability-identifier-length)
};

// NOLINTNEXTLINE(cert-err58-cpp)
TEST_CASE("structure", "[lib]") {
  MockedStroageAccess mockedStroageAccess;
  auto mockedInterpreter = std::make_unique<MockedConsoleInterpreter>();

  REQUIRE_CALL(*mockedInterpreter, process(""))
    .RETURN("");

  ConsoleController controller(std::move(mockedInterpreter));
  REQUIRE_THAT(controller.process(""), Equals(""));
}