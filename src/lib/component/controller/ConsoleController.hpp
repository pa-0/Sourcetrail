#pragma once
// internal
#include "Controller.h"

class ConsoleController final : public Controller {
public:
  explicit ConsoleController();

  ~ConsoleController() override;

};