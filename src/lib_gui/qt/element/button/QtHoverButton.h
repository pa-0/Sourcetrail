#pragma once
// Qt5
#include <QPushButton>

class QtHoverButton : public QPushButton {
  Q_OBJECT

public:
  QtHoverButton(QWidget* parent = nullptr);

signals:
  void hoveredIn(QPushButton*);
  void hoveredOut(QPushButton*);

protected:
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
};
