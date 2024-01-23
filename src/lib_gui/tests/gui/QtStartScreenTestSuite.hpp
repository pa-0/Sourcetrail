#pragma once
#include <memory>

#include <QObject>

namespace qt::window {
class QtStartScreen;
}

class QtStartScreenTestSuite : public QObject {
  Q_OBJECT
public:
  QtStartScreenTestSuite();

  ~QtStartScreenTestSuite() override;

  std::unique_ptr<qt::window::QtStartScreen> mScreen;
private slots:
  void init();

  void goodCase();
};