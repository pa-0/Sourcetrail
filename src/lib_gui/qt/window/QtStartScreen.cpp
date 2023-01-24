#include "QtStartScreen.h"
// STL
#include <algorithm>
#include <iterator>
// Boost
#include <boost/filesystem/path.hpp>
// fmt
#include <fmt/format.h>
// Qt5
#include <QAction>
#include <QCheckBox>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include <qicon.h>
// internal
#include "ApplicationSettings.h"
#include "MessageLoadProject.h"
#include "ProjectSettings.h"
#include "QtContextMenu.h"
#include "ResourcePaths.h"
#include "UserPaths.h"
#include "Version.h"
#include "globalStrings.h"
#include "utilityQt.h"

namespace {
QIcon toIcon(const std::wstring& path) {
  return QIcon(QString::fromStdWString(ResourcePaths::getGuiDirectoryPath().concatenate(path).wstr()));
}

QIcon getProjectIcon(LanguageType lang) {
  static const auto CppIcon = toIcon(L"icon/cpp_icon.png");
  static const auto CIcon = toIcon(L"icon/c_icon.png");
  static const auto PythonIcon = toIcon(L"icon/python_icon.png");
  static const auto JavaIcon = toIcon(L"icon/java_icon.png");
  static const auto ProjectIcon = toIcon(L"icon/empty_icon.png");

  switch(lang) {
#if BUILD_CXX_LANGUAGE_PACKAGE
  case LanguageType::LANGUAGE_C:
    return CIcon;
  case LANGUAGE_CPP:
    return CppIcon;
#endif    // BUILD_CXX_LANGUAGE_PACKAGE
#if BUILD_JAVA_LANGUAGE_PACKAGE
  case LANGUAGE_JAVA:
    return JavaIcon;
#endif    // BUILD_JAVA_LANGUAGE_PACKAGE
#if BUILD_PYTHON_LANGUAGE_PACKAGE
  case LANGUAGE_PYTHON:
    return PythonIcon;
#endif    // BUILD_PYTHON_LANGUAGE_PACKAGE
  case LANGUAGE_CUSTOM:
  default:
    return ProjectIcon;
  }
}
}    // namespace

QtRecentProjectButton* QtRecentProjectButton::create(QWidget* pParent) {
  auto* pButton = new QtRecentProjectButton(pParent);
  pButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
  pButton->setIcon(toIcon(L"icon/empty_icon.png"));
  pButton->setIconSize(QSize(30, 30));
  pButton->setMinimumSize(pButton->fontMetrics().boundingRect(pButton->text()).width() + 45, 40);
  pButton->setObjectName(QStringLiteral("recentButtonMissing"));
  pButton->minimumSizeHint();    // force font loading
  return pButton;
}

QtRecentProjectButton::QtRecentProjectButton(QWidget* parent) : QPushButton(parent) {}

bool QtRecentProjectButton::projectExists() const {
  return m_projectExists;
}

void QtRecentProjectButton::setProjectPath(const FilePath& projectFilePath) {
  m_projectFilePath = projectFilePath;
  m_projectExists = projectFilePath.exists();
  this->setText(QString::fromStdWString(m_projectFilePath.withoutExtension().fileName()));
  if(m_projectExists) {
    this->setToolTip(QString::fromStdWString(m_projectFilePath.wstr()));
  } else {
    const std::wstring missingFileText = L"Couldn't find " + m_projectFilePath.wstr() +
        L" in your filesystem";
    this->setToolTip(QString::fromStdWString(missingFileText));
  }
}

void QtRecentProjectButton::handleButtonClick() {
  if(m_projectExists) {
    MessageLoadProject(m_projectFilePath).dispatch();
  } else {
    QMessageBox msgBox;
    msgBox.setText(QStringLiteral("Missing Project File"));
    msgBox.setInformativeText(QString::fromStdWString(
        L"<p>Couldn't find \"" + m_projectFilePath.wstr() +
        L"\" on your filesystem.</p><p>Do you want to remove it from recent project "
        L"list?</p>"));
    msgBox.addButton(QStringLiteral("Remove"), QMessageBox::ButtonRole::YesRole);
    msgBox.addButton(QStringLiteral("Keep"), QMessageBox::ButtonRole::NoRole);
    msgBox.setIcon(QMessageBox::Icon::Question);
    const int ret = msgBox.exec();

    // QMessageBox::Yes
    if(ret == 0) {
      auto recentProjects = ApplicationSettings::getInstance()->getRecentProjects();
      for(size_t i = 0; i < recentProjects.size(); ++i) {
        if(recentProjects[i].wstr() == m_projectFilePath.wstr()) {
          recentProjects.erase(recentProjects.begin() + i);
          ApplicationSettings::getInstance()->setRecentProjects(recentProjects);
          ApplicationSettings::getInstance()->save();
          break;
        }
      }
      emit updateButtons();
    }
  }
}

void QtRecentProjectButton::contextMenuEvent(QContextMenuEvent* pEvent) {
  QtContextMenu menu(pEvent, this);

  QAction deleteAction("delete");
  connect(&deleteAction, &QAction::triggered, [this]() {
    auto recentProjects = ApplicationSettings::getInstance()->getRecentProjects();
    // Remove the current project from RecentProjects
    recentProjects.erase(
        std::remove_if(std::begin(recentProjects),
                       std::end(recentProjects),
                       [projectExists = m_projectFilePath](const auto& currentProject) {
                         return projectExists.getPath() == currentProject.getPath();
                       }),
        std::end(recentProjects));
    recentProjects.erase(
        std::remove_if(std::begin(recentProjects),
                       std::end(recentProjects),
                       [projectExists = m_projectFilePath](const auto& currentProject) {
                         return projectExists.getPath() == currentProject.getPath();
                       }),
        std::end(recentProjects));
    // Update RecentProjects
    auto pApplicationSettings = ApplicationSettings::getInstance();
    pApplicationSettings->setRecentProjects(recentProjects);
    pApplicationSettings->save(UserPaths::getAppSettingsFilePath());
    hide();

    emit updateButtons();
  });

  menu.addAction(&deleteAction);
  menu.show();
}

QtStartScreen::QtStartScreen(QWidget* parent) : QtWindow(true, parent) {}

QSize QtStartScreen::sizeHint() const {
  return QSize(600, 650);
}

void QtStartScreen::updateButtons() {
  auto recentProjects = ApplicationSettings::getInstance()->getRecentProjects();
  size_t index = 0;
  for(auto* pButton : m_recentProjectsButtons) {
    pButton->disconnect();
    if(index < recentProjects.size()) {
      const auto recentProject = recentProjects[index];
      pButton->setProjectPath(recentProject);
      const auto lang = ProjectSettings::getLanguageOfProject(recentProject);
      pButton->setIcon(getProjectIcon(lang));
      pButton->setFixedWidth(pButton->fontMetrics().boundingRect(pButton->text()).width() + 45);
      // TODO(Hussien): Should be moved to QtRecentProjectButton constructor
      connect(pButton,
              &QtRecentProjectButton::clicked,
              pButton,
              &QtRecentProjectButton::handleButtonClick);
      if(pButton->projectExists()) {
        pButton->setObjectName(QStringLiteral("recentButton"));
        connect(pButton, &QtRecentProjectButton::clicked, this, &QtStartScreen::handleRecentButton);
      } else {
        connect(pButton, &QtRecentProjectButton::updateButtons, this, &QtStartScreen::updateButtons);
        pButton->setObjectName(QStringLiteral("recentButtonMissing"));
      }
    } else {
      pButton->hide();
    }
    index++;
  }
  setStyleSheet(utility::getStyleSheet(ResourcePaths::getGuiDirectoryPath().concatenate(
                                           L"startscreen/startscreen.css"))
                    .c_str());
}

void QtStartScreen::setupStartScreen() {
  setStyleSheet(utility::getStyleSheet(ResourcePaths::getGuiDirectoryPath().concatenate(
                                           L"startscreen/startscreen.css"))
                    .c_str());
  addLogo();

  auto* pLayout = new QHBoxLayout();
  pLayout->setContentsMargins(15, 170, 15, 0);
  pLayout->setSpacing(1);
  m_content->setLayout(pLayout);

  {
    auto* pVBoxLayout = new QVBoxLayout();
    pLayout->addLayout(pVBoxLayout, 3);

    auto* pVersionLabel = new QLabel(
        fmt::format("Version {}", Version::getApplicationVersion().toDisplayString()).c_str(), this);
    pVersionLabel->setObjectName(QStringLiteral("boldLabel"));
    pVBoxLayout->addWidget(pVersionLabel);

    pVBoxLayout->addSpacing(20);

    auto* pGithubButton = new QPushButton(QStringLiteral("View on GitHub"), this);
    pGithubButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pGithubButton->setObjectName(QStringLiteral("infoButton"));
    pGithubButton->setIcon(toIcon(L"startscreen/github_icon.png"));
    pGithubButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pGithubButton, &QPushButton::clicked, []() {
      QDesktopServices::openUrl(QUrl("github"_g, QUrl::TolerantMode));
    });
    pVBoxLayout->addWidget(pGithubButton);

    pVBoxLayout->addSpacing(35);
    pVBoxLayout->addStretch();

    auto* pNewProjectButton = new QPushButton(QStringLiteral("New Project"), this);
    pNewProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pNewProjectButton->setObjectName(QStringLiteral("projectButton"));
    pNewProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pNewProjectButton, &QPushButton::clicked, this, &QtStartScreen::handleNewProjectButton);
    pVBoxLayout->addWidget(pNewProjectButton);

    pVBoxLayout->addSpacing(8);

    auto* pOpenProjectButton = new QPushButton(QStringLiteral("Open Project"), this);
    pOpenProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pOpenProjectButton->setObjectName(QStringLiteral("projectButton"));
    pOpenProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pOpenProjectButton, &QPushButton::clicked, this, &QtStartScreen::handleOpenProjectButton);
    pVBoxLayout->addWidget(pOpenProjectButton);
  }

  pLayout->addSpacing(50);

  { // Create Recent Projects
    auto* pVBoxLayout = new QVBoxLayout();
    pLayout->addLayout(pVBoxLayout, 1);

    auto* pRecentProjectsLabel = new QLabel(QStringLiteral("Recent Projects: "), this);
    pRecentProjectsLabel->setObjectName(QStringLiteral("titleLabel"));
    pVBoxLayout->addWidget(pRecentProjectsLabel);

    pVBoxLayout->addSpacing(20);

    for(size_t i = 0; i < ApplicationSettings::getInstance()->getMaxRecentProjectsCount(); ++i) {
      auto pButton = QtRecentProjectButton::create(this);
      m_recentProjectsButtons.push_back(pButton);
      pVBoxLayout->addWidget(pButton);
    }

    pVBoxLayout->addStretch();
  }

  pLayout->addStretch(1);

  updateButtons();

  const QSize size = sizeHint();
  move(parentWidget()->width() / 2 - size.width() / 2,
       parentWidget()->height() / 2 - size.height() / 2);
}

void QtStartScreen::handleNewProjectButton() {
  emit openNewProjectDialog();
}

void QtStartScreen::handleOpenProjectButton() {
  emit openOpenProjectDialog();
}

void QtStartScreen::handleRecentButton() {
  emit finished();
}