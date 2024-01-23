#pragma once

#include <QScrollArea>
#include <QTableWidget>

#include "QtWindow.h"

class QtShortcutTable : public QTableWidget {
  Q_OBJECT
public:
  QtShortcutTable(QWidget* parent = nullptr);
  void updateSize();

protected:
  void wheelEvent(QWheelEvent* event) override;
};

class QtKeyboardShortcuts : public QtWindow {
  Q_OBJECT
public:
  QtKeyboardShortcuts(QWidget* parent = nullptr);
  ~QtKeyboardShortcuts() override;

  [[nodiscard]] QSize sizeHint() const override;

  void populateWindow(QWidget* widget) override;
  void windowReady() override;

private:
  struct Shortcut {
    const QString name;
    const QString shortcut;

    Shortcut(const QString& name, const QString& shortcut);
    static Shortcut defaultOrMac(const QString& name, const QString& defaultShortcut, const QString& macShortcut);
    static Shortcut winMacOrLinux(const QString& name,
                                  const QString& winShortcut,
                                  const QString& macShortcut,
                                  const QString& linuxShortcut);
  };

  QtShortcutTable* createTableWidget(const std::string& objectName);
  void addShortcuts(QtShortcutTable* table, const std::vector<Shortcut>& shortcuts) const;

  QTableWidget* createGeneralShortcutsTable();
  QTableWidget* createCodeViewShortcutsTable();
  QTableWidget* createGraphViewShortcutsTable();
};