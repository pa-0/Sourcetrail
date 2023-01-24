#include "QtStartScreen.h"
// fmt
#include <fmt/format.h>
// Qt5
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
// internal
#include "ApplicationSettings.h"
#include "LanguageType.h"
#include "ProjectSettings.h"
#include "QtRecentProjectButton.h"
#include "ResourcePaths.h"
#include "Version.h"
#include "globalStrings.h"
#include "utilityQt.h"

namespace {

QIcon getProjectIcon(LanguageType lang) {
  static const auto CppIcon = utility::toIcon(L"icon/cpp_icon.png");
  static const auto CIcon = utility::toIcon(L"icon/c_icon.png");
  static const auto PythonIcon = utility::toIcon(L"icon/python_icon.png");
  static const auto JavaIcon = utility::toIcon(L"icon/java_icon.png");
  static const auto ProjectIcon = utility::toIcon(L"icon/empty_icon.png");

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
        connect(pButton, &QtRecentProjectButton::clicked, [this]() { emit finished(); });
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
    pGithubButton->setIcon(utility::toIcon(L"startscreen/github_icon.png"));
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
    connect(pNewProjectButton, &QPushButton::clicked, [this]() { emit openNewProjectDialog(); });
    pVBoxLayout->addWidget(pNewProjectButton);

    pVBoxLayout->addSpacing(8);

    auto* pOpenProjectButton = new QPushButton(QStringLiteral("Open Project"), this);
    pOpenProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pOpenProjectButton->setObjectName(QStringLiteral("projectButton"));
    pOpenProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pOpenProjectButton, &QPushButton::clicked, [this]() { emit openOpenProjectDialog(); });
    pVBoxLayout->addWidget(pOpenProjectButton);
  }

  pLayout->addSpacing(50);

  {    // Create Recent Projects
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