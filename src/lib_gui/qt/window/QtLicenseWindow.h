#pragma once
// internal
#include "QtWindow.h"

class QtLicenseWindow : public QtWindow {
  Q_OBJECT
public:
  QtLicenseWindow(QWidget* pParent = nullptr);
  QSize sizeHint() const override;

protected:
  // QtWindow implementation
  virtual void populateWindow(QWidget* widget) override;
  virtual void windowReady() override;
};