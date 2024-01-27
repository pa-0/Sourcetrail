#pragma once
#include <vector>

#include <QAbstractListModel>
#include <QIcon>

#include "FilePath.h"

namespace qt::element::model {

struct RecentItem final {
  bool exists;
  QString title;
  QIcon icon;
  FilePath path;
};

struct RecentItemModel final : QAbstractListModel {
  RecentItemModel(const std::vector<FilePath>& recentProjects, size_t maxRecentProjects, QObject* parent = nullptr);

  [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override {
    std::ignore = parent;
    return static_cast<int>(std::min(mMaxRecentProjects, mRecentProjects.size()));
  }

  void clicked(const QModelIndex& index);

  void removeItem(int index) {
    if(static_cast<size_t>(index) >= mRecentProjects.size()) {
      return;
    }

    mRecentProjects.erase(std::begin(mRecentProjects) + index);
  }

private:
  size_t mMaxRecentProjects = 0;
  std::vector<RecentItem> mRecentProjects;
};

}    // namespace qt::element::model
