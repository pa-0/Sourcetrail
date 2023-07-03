#pragma once
// Qt5
#include <qglobal.h>
// internal
#include "ViewWidgetWrapper.h"

QT_FORWARD_DECLARE_CLASS(QWidget)

class View;

class QtViewWidgetWrapper : public ViewWidgetWrapper {
public:
  static QWidget* getWidgetOfView(const View* pView);

  explicit QtViewWidgetWrapper(QWidget* pWidget);

  ~QtViewWidgetWrapper() override;

  [[nodiscard]] QWidget* getWidget() const;

private:
  QWidget* m_pWidget;
};