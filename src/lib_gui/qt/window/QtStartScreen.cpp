#include "QtStartScreen.hpp"

#include <utility>

#include <fmt/format.h>

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListView>
#include <QMenu>
#include <QPushButton>

#include "ApplicationSettings.h"
#include "RecentItemModel.hpp"
#include "Version.h"
#include "globalStrings.h"
#include "utilityQt.h"

namespace {

QPushButton* createButton(qt::window::QtStartScreen* that,
                          const QString& buttonName,
                          const QString& objectName,
                          std::function<void()> onClickEvent) {
  auto* newProjectButton = new QPushButton(buttonName, that);    // NOLINT(cppcoreguidelines-owning-memory)
#if Q_OS_MAC
  newProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
#endif
  newProjectButton->setObjectName(objectName);
  newProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  QObject::connect(newProjectButton, &QPushButton::clicked, that, std::move(onClickEvent));
  return newProjectButton;
}

}    // namespace

namespace qt::window {

QtStartScreen::QtStartScreen(QWidget* pParent) : QtWindow(true, pParent) {}

QSize QtStartScreen::sizeHint() const {
  constexpr auto Size = QSize(600, 650);
  return Size;
}

void QtStartScreen::setupStartScreen() {
  addLogo();

  // Create the main layout
  auto* layout = new QHBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory)
  layout->setContentsMargins(15, 170, 15, 0);
  m_content->setLayout(layout);

  createVersionAndGithub(layout);

  layout->addSpacing(50);

  createRecentProjects(layout);

  // Move the window to center of the parent window.
  const QSize size = sizeHint();
  move(parentWidget()->width() / 2 - size.width() / 2, parentWidget()->height() / 2 - size.height() / 2);

  setStyleSheet(utility::getStyleSheet("://startscreen/startscreen.css"));
}

void QtStartScreen::createRecentProjects(QHBoxLayout* layout) {
  auto* vBoxLayout = new QVBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory)
  layout->addLayout(vBoxLayout, 1);

  auto* recentProjectsLabel = new QLabel(QStringLiteral("Recent Projects: "), this);
  recentProjectsLabel->setObjectName(QStringLiteral("titleLabel"));
  vBoxLayout->addWidget(recentProjectsLabel);

  vBoxLayout->addSpacing(20);

  auto maxRecentProjectsCount = ApplicationSettings::getInstance()->getMaxRecentProjectsCount();
  auto recentProjects = ApplicationSettings::getInstance()->getRecentProjects();

  auto* viewList = new QListView;    // NOLINT(cppcoreguidelines-owning-memory)
  mRecentModel = new element::model::RecentItemModel(
      recentProjects, maxRecentProjectsCount);    // NOLINT(cppcoreguidelines-owning-memory)
  connect(viewList, &QListView::clicked, mRecentModel, &element::model::RecentItemModel::clicked);
  connect(viewList, &QListView::customContextMenuRequested, this, [viewList, this](const QPoint& point) {
    QMenu contextMenu(tr("Context menu"), viewList);

    QAction action1(tr("Delete"), viewList);
    QObject::connect(&action1, &QAction::triggered, viewList, [viewList, point, this](auto) {
      const auto index = viewList->indexAt(point);
      if(index.isValid()) {
        mRecentModel->removeItem(index.row());
      }
    });
    contextMenu.addAction(&action1);

    contextMenu.exec(viewList->mapToGlobal(point));
  });
  viewList->setIconSize(QSize(30, 30));
  viewList->setModel(mRecentModel);
  viewList->setDragDropMode(QAbstractItemView::InternalMove);
  viewList->setMovement(QListView::Snap);
  viewList->setDefaultDropAction(Qt::MoveAction);
  viewList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  viewList->setUniformItemSizes(true);
  viewList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  viewList->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
  vBoxLayout->addWidget(viewList, 1);

  vBoxLayout->addStretch();
}

void QtStartScreen::createVersionAndGithub(QHBoxLayout* layout) {
  auto* vBoxLayout = new QVBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory)
  layout->addLayout(vBoxLayout, 3);

  // Create a Version label
  auto* pVersionLabel = new QLabel(fmt::format("Version {}", Version::getApplicationVersion().toString()).c_str(), this);
  pVersionLabel->setObjectName(QStringLiteral("boldLabel"));
  vBoxLayout->addWidget(pVersionLabel);

  vBoxLayout->addSpacing(20);

  // Create a GitHub button
  auto* githubButton = createButton(this, QStringLiteral("View on GitHub"), QStringLiteral("infoButton"), []() {
    QDesktopServices::openUrl(QUrl("github"_g, QUrl::TolerantMode));
  });
  githubButton->setIcon(QIcon("://startscreen/github_icon.png"));
  vBoxLayout->addWidget(githubButton);

  vBoxLayout->addSpacing(35);
  vBoxLayout->addStretch();

  vBoxLayout->addWidget(createButton(
      this, QStringLiteral("New Project"), QStringLiteral("projectButton"), [this]() { emit openNewProjectDialog(); }));
  vBoxLayout->addSpacing(8);
  vBoxLayout->addWidget(createButton(
      this, QStringLiteral("Open Project"), QStringLiteral("projectButton"), [this]() { emit openOpenProjectDialog(); }));
}

}    // namespace qt::window