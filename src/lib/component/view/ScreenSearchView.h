#pragma once

#include "View.h"

class ScreenSearchView : public View {
public:
  explicit ScreenSearchView(ViewLayout* viewLayout);
  ~ScreenSearchView() override;

  // View implementation
  [[nodiscard]] virtual std::string getName() const;

  virtual void setMatchCount(size_t matchCount) = 0;
  virtual void setMatchIndex(size_t matchIndex) = 0;

  virtual void addResponder(const std::string& name) = 0;
};