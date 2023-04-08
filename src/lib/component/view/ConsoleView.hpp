#pragma once
// STL
#include <string>
// internal
#include "View.h"

class ConsoleView : public View {
public:
  explicit ConsoleView(ViewLayout* pViewLayout);

  ~ConsoleView() override;

  [[nodiscard]] std::string getName() const override;
};