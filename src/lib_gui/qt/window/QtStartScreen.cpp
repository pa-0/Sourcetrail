#include "QtStartScreen.h"

#include <format>

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "ApplicationSettings.h"
#include "LanguageType.h"
#include "ProjectSettings.h"
#include "QtRecentProjectButton.h"
#include "Version.h"
#include "globalStrings.h"
#include "utilityQt.h"

namespace {

QIcon getProjectIcon(LanguageType lang) {
  static const auto CppIcon = QIcon("://icon/cpp_icon.png");
  static const auto CIcon = QIcon("://icon/c_icon.png");
  static const auto ProjectIcon = QIcon("://icon/empty_icon.png");

  switch(lang) {
#if BUILD_CXX_LANGUAGE_PACKAGE
  case LanguageType::LANGUAGE_C:
    return CIcon;
  case LANGUAGE_CPP:
    return CppIcon;
#endif
  case LANGUAGE_CUSTOM:
  default:
    return ProjectIcon;
  }
}

QtRecentProjectButton* createRecentProjectButton(QWidget* pParent) {
  constexpr auto IconSize = QSize(30, 30);

  auto* pButton = new QtRecentProjectButton(pParent);    // NOLINT(cppcoreguidelines-owning-memory)
  pButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
  pButton->setIcon(getProjectIcon(LANGUAGE_CUSTOM));
  pButton->setIconSize(IconSize);
  pButton->setMinimumSize(pButton->fontMetrics().boundingRect(pButton->text()).width() + 45, 40);
  pButton->setObjectName(QStringLiteral("recentButtonMissing"));
  pButton->minimumSizeHint();    // force font loading

  return pButton;
}

}    // namespace

namespace qt::window {

QtStartScreen::QtStartScreen(QWidget* pParent) : QtWindow(true, pParent) {}

QSize QtStartScreen::sizeHint() const {
  constexpr auto Size = QSize(600, 650);
  return Size;
}

void QtStartScreen::updateButtons() {
  auto recentProjects = ApplicationSettings::getInstance()->getRecentProjects();
  size_t index = 0;
  for(auto* pButton : m_recentProjectsButtons) {
    // make sure that this button is used safely
    pButton->disconnect();

    if(index < recentProjects.size()) {
      const auto recentProject = recentProjects[index];
      pButton->setProjectPath(recentProject);
      const auto lang = ProjectSettings::getLanguageOfProject(recentProject);
      pButton->setIcon(getProjectIcon(lang));
      pButton->setFixedWidth(pButton->fontMetrics().boundingRect(pButton->text()).width() + 45);
      // NOTE: Can not be moved to QtRecentProjectButton coz every updateButtons will remove the
      // connect again.
      connect(pButton, &QtRecentProjectButton::clicked, pButton, &QtRecentProjectButton::handleButtonClick);
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
}

void QtStartScreen::setupStartScreen() {
  addLogo();

  // Create the main layout
  auto* pLayout = new QHBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory)
  pLayout->setContentsMargins(15, 170, 15, 0);
  pLayout->setSpacing(1);
  m_content->setLayout(pLayout);

  {
    auto* pVBoxLayout = new QVBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory)
    pLayout->addLayout(pVBoxLayout, 3);

    // Create a Version label
    auto* pVersionLabel = new QLabel(std::format("Version {}", Version::getApplicationVersion().toDisplayString()).c_str(), this);
    pVersionLabel->setObjectName(QStringLiteral("boldLabel"));
    pVBoxLayout->addWidget(pVersionLabel);

    pVBoxLayout->addSpacing(20);

    // Create a github button
    auto* pGithubButton = new QPushButton(QStringLiteral("View on GitHub"), this);
    pGithubButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pGithubButton->setObjectName(QStringLiteral("infoButton"));
    pGithubButton->setIcon(utility::toIcon(L"startscreen/github_icon.png"));
    pGithubButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pGithubButton, &QPushButton::clicked, this, []() { QDesktopServices::openUrl(QUrl("github"_g, QUrl::TolerantMode)); });
    pVBoxLayout->addWidget(pGithubButton);

    pVBoxLayout->addSpacing(35);
    pVBoxLayout->addStretch();

    // Create a new project button
    auto* pNewProjectButton = new QPushButton(QStringLiteral("New Project"), this);
    pNewProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pNewProjectButton->setObjectName(QStringLiteral("projectButton"));
    pNewProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pNewProjectButton, &QPushButton::clicked, [this]() { emit openNewProjectDialog(); });
    pVBoxLayout->addWidget(pNewProjectButton);

    pVBoxLayout->addSpacing(8);

    // Create a open project button
    auto* pOpenProjectButton = new QPushButton(QStringLiteral("Open Project"), this);
    pOpenProjectButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);    // fixes layouting on Mac
    pOpenProjectButton->setObjectName(QStringLiteral("projectButton"));
    pOpenProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pOpenProjectButton, &QPushButton::clicked, [this]() { emit openOpenProjectDialog(); });
    pVBoxLayout->addWidget(pOpenProjectButton);
  }

  pLayout->addSpacing(50);

  {                                         // Create Recent Projects
    auto* pVBoxLayout = new QVBoxLayout;    // NOLINT(cppcoreguidelines-owning-memory)
    pLayout->addLayout(pVBoxLayout, 1);

    auto* pRecentProjectsLabel = new QLabel(QStringLiteral("Recent Projects: "), this);
    pRecentProjectsLabel->setObjectName(QStringLiteral("titleLabel"));
    pVBoxLayout->addWidget(pRecentProjectsLabel);

    pVBoxLayout->addSpacing(20);

    for(size_t index = 0; index < ApplicationSettings::getInstance()->getMaxRecentProjectsCount(); ++index) {
      auto* pButton = createRecentProjectButton(this);
      m_recentProjectsButtons.push_back(pButton);
      pVBoxLayout->addWidget(pButton);
    }

    pVBoxLayout->addStretch();
  }

  pLayout->addStretch(1);

  updateButtons();

  // Move the window to center of the parent window.
  const QSize size = sizeHint();
  move(parentWidget()->width() / 2 - size.width() / 2, parentWidget()->height() / 2 - size.height() / 2);

  setStyleSheet(utility::getStyleSheet("://startscreen/startscreen.css"));
}

}    // namespace qt::window