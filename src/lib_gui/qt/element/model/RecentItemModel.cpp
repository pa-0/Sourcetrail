#include "RecentItemModel.hpp"

#include <tuple>

#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include <QMenu>
#include <QMessageBox>

#include "MessageLoadProject.h"
#include "ProjectSettings.h"

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

int showMissingProjectDialog(const std::wstring& projectFilePath) {
  QMessageBox msgBox;
  msgBox.setText(QStringLiteral("Missing Project File"));
  msgBox.setInformativeText(QString::fromStdWString(L"<p>Couldn't find \"" + projectFilePath +
                                                    L"\" on your filesystem.</p><p>Do you want to remove it from recent project "
                                                    L"list?</p>"));
  msgBox.addButton(QStringLiteral("Remove"), QMessageBox::ButtonRole::YesRole);
  msgBox.addButton(QStringLiteral("Keep"), QMessageBox::ButtonRole::NoRole);
  msgBox.setIcon(QMessageBox::Icon::Question);
  return msgBox.exec();
}

}    // namespace

namespace qt::element::model {

RecentItemModel::RecentItemModel(const std::vector<FilePath>& recentProjects, size_t maxRecentProjects, QObject* parent)
    : QAbstractListModel(parent), mMaxRecentProjects(maxRecentProjects) {
  mRecentProjects = recentProjects | ranges::views::transform([](const FilePath& recentProject) -> RecentItem {
                      const auto lang = ProjectSettings::getLanguageOfProject(recentProject);
                      return {recentProject.exists(),
                              QString::fromStdWString(recentProject.withoutExtension().fileName()),
                              getProjectIcon(lang),
                              recentProject};
                    }) |
      ranges::to<std::vector<RecentItem>>;
}

QVariant RecentItemModel::data(const QModelIndex& index, int role) const {
  if(!index.isValid()) {
    return {};
  }

  const auto idx = static_cast<size_t>(index.row());
  if(idx >= mRecentProjects.size()) {
    return {};
  }

  const auto& item = mRecentProjects[static_cast<size_t>(idx)];
  switch(role) {
  case Qt::DisplayRole:
    return item.title;
  case Qt::DecorationRole:
    return item.icon;
  case Qt::ToolTipRole:
    return item.exists ? QVariant {} : QString("\"%0\" is missed").arg(QString::fromStdWString(item.path.wstr()));
  case Qt::FontRole: {
    QFont font;
    font.setStrikeOut(true);
    return item.exists ? QVariant {} : font;
  }
  default:
    break;
  }

  return {};
}

void RecentItemModel::clicked(const QModelIndex& index) {
  if(!index.isValid()) {
    return;
  }

  const auto idx = static_cast<size_t>(index.row());
  if(idx >= mRecentProjects.size()) {
    return;
  }

  const auto& item = mRecentProjects[idx];
  if(item.exists) {
    MessageLoadProject(item.path).dispatch();
    return;
  }
  const auto status = ::showMissingProjectDialog(item.path.wstr());
  if(status == 0) {
    mRecentProjects.erase(std::begin(mRecentProjects) + index.row());
  }
}

}    // namespace qt::element::model