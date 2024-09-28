#include <filesystem>

#include <fmt/color.h>
#include <fmt/printf.h>

#include <range/v3/algorithm/find_first_of.hpp>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTimer>

#include "ProjectSettings.h"
#include "StorageCache.h"
#include "Application.h"
#include "logging.h"
#include "Version.h"

namespace fs = std::filesystem;

void printFindHelp() {
  fmt::print("Find help:\n"
  "\t function\n");
}

int main(int argc, char* argv[]) {
  const QCoreApplication application(argc, argv);

  QCoreApplication::setApplicationName("Sourcetrail");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument(
      "compile_commands", QCoreApplication::translate("compile_commands", "A path to compile_commands.json"));

  parser.process(application);

  const auto args = parser.positionalArguments();
  if(args.isEmpty()) {
    fmt::print(fg(fmt::color::red), "ERROR: Missing the following argument compile_commands.");
    return EXIT_FAILURE;
  }

  std::error_code errorCode;
  const fs::path compileCommandsPath = args.at(0).toStdString();
  if(!fs::exists(compileCommandsPath, errorCode) && !fs::is_regular_file(compileCommandsPath, errorCode)) {
    fmt::print(fg(fmt::color::red), "ERROR: The compileCommands path is not exists or not at file.");
    return EXIT_FAILURE;
  }

  auto* logger = spdlog::default_logger_raw();
  logger->set_level(spdlog::level::off);

  Application::createInstance(Version{}, nullptr, nullptr);
  MessageLoadProject(FilePath{compileCommandsPath.wstring()}).dispatchImmediately();

  QTimer::singleShot(0, QCoreApplication::instance(), []() {
    auto* storage = Application::getInstance()->getStorageCacheRaw();
    bool running = true;
    fmt::print("Welcome to Sourcetrail\n");
    while(running) {
      fmt::print("> ");
      std::string line;
      std::getline(std::cin, line);
      auto lines = utility::splitToVector(line, ' ');
      if(lines.empty()) {
        continue;
      }
      if("quit" == lines.front() || "exit" == lines.front()) {
        running = false;
      }
      if("find" == lines.front()) {
        if(lines.size() == 1) {
          printFindHelp();
        }
        if("function" == *(lines.begin() + 1)) {
          storage->getAvailableNodeTypes();
          fmt::print("Found 1 function:\n\tmain:25\n");
        }
      }
    }
    fmt::print("Bye!!\n");
    Application::destroyInstance();
    QCoreApplication::exit();
  });

  return QCoreApplication::exec();
}