#include "QtPluginsWindow.h"
// STL
#include <filesystem>
// fmt
#include <fmt/format.h>
// Qt5
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QTimer>
// Internal
#include "QtKeyboardShortcuts.h"
#include "logging.h"

namespace fs = std::filesystem;

namespace {
std::string getDynamicLibrary() {
#ifdef _WIN32
  return "dll";
#else
  return "so";
#endif
}

QtShortcutTable* createTableWidget(const std::string& objectName, QWidget* pParent) {
  auto* pTable = new QtShortcutTable(pParent);
  pTable->setObjectName(objectName.c_str());

  pTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
  pTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
  pTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

  pTable->setShowGrid(true);
  pTable->setAlternatingRowColors(true);
  pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
  pTable->verticalHeader()->hide();

  pTable->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

  pTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  pTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  pTable->setColumnCount(2);
  pTable->setHorizontalHeaderItem(0, new QTableWidgetItem(QStringLiteral("Name")));
  pTable->setHorizontalHeaderItem(1, new QTableWidgetItem(QStringLiteral("Status")));

  return pTable;
}
}    // namespace

QtPluginsWindow::QtPluginsWindow(QWidget* pParent) noexcept : QtWindow(false, pParent) {
  setScrollAble(true);
}

QtPluginsWindow::~QtPluginsWindow() = default;

QSize QtPluginsWindow::sizeHint() const {
  return {800, 600};
}

void QtPluginsWindow::populateWindow(QWidget* pWidget) {
  auto* pLayout = new QVBoxLayout(pWidget);

  auto* pTable = createTableWidget("table_graph", this);

  std::vector<std::pair<std::string, QTableWidgetItem*>> plugins {{"clang", nullptr},
                                                           {"java", nullptr},
                                                           {"python", nullptr},
                                                           {"csharp", nullptr},
                                                           {"rust", nullptr}};

  pTable->setRowCount(static_cast<int>(plugins.size()));
  uint32_t index = 0;
  for(auto& [pluginName, pLabel] : plugins) {
    pTable->setItem(index, 0, new QTableWidgetItem(QString::fromStdString(pluginName)));
    pLabel = new QTableWidgetItem(QString::fromStdString("pinding"));
    pLabel->setTextAlignment(Qt::AlignHCenter);
    pTable->setItem(index, 1, pLabel);
    ++index;
  }
  pLayout->addWidget(pTable);

  pWidget->setLayout(pLayout);

  // Call later
  QTimer::singleShot(1000, [plugins=std::move(plugins)] {
    std::error_code errorCode;
    for(auto& [pluginName, pLabel] : plugins) {
      const auto pluginDir = fs::path {"plugins"};
      auto pluginFile = fmt::format("{}.{}", (pluginDir / pluginName).string(), getDynamicLibrary());
      pLabel->setText((fs::exists(pluginFile, errorCode) ? "found" : "not found"));
    }
  });
}

void QtPluginsWindow::windowReady() {
  updateTitle(QStringLiteral("Plugins"));
  updateCloseButton(QStringLiteral("Close"));

  setNextVisible(false);
  setPreviousVisible(false);
}