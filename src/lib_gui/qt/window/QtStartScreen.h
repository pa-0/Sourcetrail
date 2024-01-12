#pragma once
// Internal
#include "QtWindow.h"

class QtRecentProjectButton;

namespace qt::window {
/**
 * @brief Welcome screen
 *
 * It's a pop-up window which show in top of the main application.
 *
 *   ┌─────────┐
 *   │         │
 *┌──┼  Logo   ┼────────────────┐
 *│  │         │                │
 *│  └─────────┘                │
 *│                             │
 *│  ┌version─┐      ┌Recent──┐ │
 *│  │ Github │      │        │ │
 *│  └────────┘      │        │ │
 *│                  │        │ │
 *│ ┌───────┐        │        │ │
 *│  New Pro         └────────┘ │
 *│  Open Pro                   │
 *│ └───────┘                   │
 *└─────────────────────────────┘
 */
class QtStartScreen final : public QtWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructor.
   *
   * @param pParent parent object parent.
   */
  explicit QtStartScreen(QWidget* pParent = nullptr);

  /**
   * @brief Recommended window size.
   *
   * @return Recommended window size.
   */
  [[nodiscard]] QSize sizeHint() const override;

  /**
   * @brief Initialize the startup window elements.
   */
  void setupStartScreen();

signals:
  /**
   * @brief A signal emitted when the open button clicked.
   */
  void openOpenProjectDialog();

  /**
   * @brief A signal emitted when the new button clicked.
   */
  void openNewProjectDialog();

private slots:
  /**
   * @brief Fill/Refresh the recent projects layout.
   *
   * @note it's called when a new project added or when a project deleted.
   */
  void updateButtons();

private:                                                          // NOLINT(readability-redundant-access-specifiers)
  std::vector<QtRecentProjectButton*> m_recentProjectsButtons;    ///< A list of buttons for recent projects
};

}    // namespace qt::window