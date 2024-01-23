#pragma once
// Qt5
#include <QPushButton>
// internal
#include "FilePath.h"

class QtIconButton : public QPushButton {
  Q_OBJECT
public:
  QtIconButton(const FilePath& iconPath, const FilePath& hoveredIconPath, QWidget* parent = nullptr);
  ~QtIconButton() = default;

  void setColor(QColor color);

protected:
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);

private:
  void setIconFromPath(const FilePath& path);

  const FilePath m_iconPath;
  const FilePath m_hoveredIconPath;

  QColor m_color;
};
