#pragma once
// Internal
#include "QtWindow.h"

class QtPluginsWindow final : public QtWindow {
  Q_OBJECT
public:
  explicit QtPluginsWindow(QWidget* pParent = nullptr) noexcept;

  ~QtPluginsWindow() override;

  QSize sizeHint() const override;

protected:
  // QtWindow implementation
  void populateWindow(QWidget* pWidget) override;

  void windowReady() override;
};