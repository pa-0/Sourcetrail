#pragma once

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>

#include "Bookmark.h"
#include "BookmarkController.h"
#include "ControllerProxy.h"

class Bookmark;

class QtBookmark : public QFrame {
  Q_OBJECT

public:
  explicit QtBookmark(ControllerProxy<BookmarkController>* controllerProxy);
  ~QtBookmark() override;

  void setBookmark(std::shared_ptr<Bookmark> bookmark);
  [[nodiscard]] Id getBookmarkId() const;

  [[nodiscard]] QTreeWidgetItem* getTreeWidgetItem() const;
  void setTreeWidgetItem(QTreeWidgetItem* treeWidgetItem);

public slots:
  void commentToggled();

protected:
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;

  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;

private slots:
  void activateClicked();
  void editClicked();
  void deleteClicked();
  void elideButtonText();

private:
  void updateArrow();

  [[nodiscard]] std::string getDateString() const;

  ControllerProxy<BookmarkController>* m_controllerProxy;

  QPushButton* m_activateButton;
  QPushButton* m_editButton;
  QPushButton* m_deleteButton;
  QPushButton* m_toggleCommentButton;

  QLabel* m_comment;
  QLabel* m_dateLabel;

  std::shared_ptr<Bookmark> m_bookmark;

  // pointer to the bookmark category item in the treeView, allows to refresh tree view when a
  // node changes in size (e.g. toggle comment). Not a nice solution to the problem, but couldn't
  // find anything better yet. (sizeHintChanged signal can't be emitted here...)
  QTreeWidgetItem* m_treeWidgetItem;

  std::wstring m_arrowImageName;
  bool m_hovered;

  bool m_ignoreNextResize;
};