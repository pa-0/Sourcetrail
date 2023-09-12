#pragma once
#include <QObject>
#include <QtTest>

struct QtSelfRefreshIconButtonTest final : QObject {
  Q_OBJECT
private slots:
  void testingConstruct();
  void testingConstructWithEmptyString();
  void testingSetText();
  void testingSetTextWithEmptyString();
  void testingSetIconPath();
  void testingSetIconPathWithEmptyString();
};