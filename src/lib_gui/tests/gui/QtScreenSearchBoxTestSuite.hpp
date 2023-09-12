#pragma once
#include <QObject>
#include <QtTest>

#include "ControllerProxy.h"
#include "MockedView.hpp"
#include "MockedViewLayout.hpp"
#include "QtScreenSearchBox.h"
#include "ScreenSearchController.h"

struct QtScreenSearchBoxTest final : QObject {
  Q_OBJECT

  std::unique_ptr<MockedViewLayout> viewLayout;
  std::unique_ptr<MockedView> view;
  std::unique_ptr<ControllerProxy<ScreenSearchController>> controllerProxy;
  std::unique_ptr<QtScreenSearchBox> searchBox;
private slots:
  void init();
  void cleanup();
  void testingStructure();
  void testingSetMatchCountZero();
  void testingSetMatchCount();
  void testingSetMatchIndexZero();
  void testingSetMatchIndex();
  void testingAddResponderEmpty();
  void testingAddResponderExists();
  void testingCloseSignal();
  void testingTextChangedSignal();
  void testingReturnPressedSignal();
};
