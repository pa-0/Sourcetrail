#pragma once
// internal
#include "Controller.h"

class StorageAccess;

class ConsoleController final : public Controller {
public:
  explicit ConsoleController(StorageAccess* pViewLayout);

  ~ConsoleController() override;

  void clear() override;
};