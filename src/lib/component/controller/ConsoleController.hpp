#pragma once
// STL
#include <memory>
#include <string>
// internal
#include "Controller.h"

class ConsoleParser;
class StorageAccess;

class ConsoleController final : public Controller {
public:
  explicit ConsoleController(StorageAccess* pStorageAccess);

  ConsoleController(const ConsoleController& pViewLayout) = delete;
  ConsoleController(ConsoleController&& pViewLayout) = delete;
  ConsoleController& operator=(const ConsoleController& pViewLayout) = delete;
  ConsoleController& operator=(ConsoleController&& pViewLayout) = delete;

  ~ConsoleController() override;

  std::string process(const std::string& text);

  void clear() override;

private:
  std::unique_ptr<ConsoleParser> m_pParser;
};