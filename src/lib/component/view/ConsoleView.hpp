#pragma once
// STL
#include <string>
// internal
#include "View.h"
#include "ControllerProxy.h"
#include "ConsoleController.hpp"

class ConsoleView : public View {
public:
  explicit ConsoleView(ViewLayout* pViewLayout);

  ~ConsoleView() override;

  [[nodiscard]] std::string getName() const override;

private:
  const ControllerProxy<ConsoleController> m_pControllerProxy;
};