// Qt5
#include <QPushButton>
// Internal
#include "FilePath.h"

class QtRecentProjectButton : public QPushButton {
  Q_OBJECT

public:
  static QtRecentProjectButton* create(QWidget* pParent);

  explicit QtRecentProjectButton(QWidget* pParent);

  bool projectExists() const;

  void setProjectPath(const FilePath& projectFilePath);

public slots:
  void handleButtonClick();

signals:
  void updateButtons();

protected:
  void contextMenuEvent(QContextMenuEvent* pEvent) override;

private:
  bool m_projectExists = false;
  FilePath m_projectFilePath;
};