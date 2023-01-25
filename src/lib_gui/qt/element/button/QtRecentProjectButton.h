// Qt5
#include <QPushButton>
// Internal
#include "FilePath.h"

/**
 * @brief A Button to open recent projects.
 * 
 * @see QtStartScreen.
 *
 * @code {.cpp}
 * auto* pButton = QtRecentProjectButton(pParent);
 * pButton->setProjectPath(pathToProject);
 * connect(pButton, &QPushButton::clicked,
 *         pButton, &QtRecentProjectButton::handleButtonClick);
 * connect(pButton, &QtRecentProjectButton::updateButtons,
 *         pParent, &SomeWidget::updateButtons);
 * @endcode
 */
class QtRecentProjectButton : public QPushButton {
  Q_OBJECT

public:
  /**
   * @brief Constructor.
   *
   * @param pParent parent object parent.
   */
  explicit QtRecentProjectButton(QWidget* pParent = nullptr);

  /**
   * @brief Check if the project exists.
   *
   * @note projectFilePath must be called first. otherwise it will always return false.
   *
   * @return bool return true if the project exists.
   */
  bool projectExists() const;

  /**
   * @brief Set the Project Path object.
   *
   * @param projectFilePath Path for project file.
   */
  void setProjectPath(const FilePath& projectFilePath);

public slots:
  /**
   * @brief A slot to button click.
   */
  void handleButtonClick();

signals:
  /**
   * @brief A signal emitted when the butten status changed.
   *
   * @note In some cases the project is not exists so the button needed to be removed.
   */
  void updateButtons();

protected:
  /**
   * @brief This is implemented to receive widget context menu events.
   *
   * @param pEvent Context Menu Event
   */
  void contextMenuEvent(QContextMenuEvent* pEvent) override;

private:
  bool m_projectExists = false;    ///< A value to project exists. check setProjectPath.
  FilePath m_projectFilePath;      ///< A path to the project. check setProjectPath.
};