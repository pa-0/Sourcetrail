#pragma once

#include "SqliteStorage.h"
#include "StorageBookmark.h"
#include "StorageBookmarkCategory.h"
#include "StorageBookmarkedEdge.h"
#include "StorageBookmarkedNode.h"
#include "types.h"

class SqliteBookmarkStorage final : public SqliteStorage {
public:
  explicit SqliteBookmarkStorage(const FilePath& dbFilePath);

  size_t getStaticVersion() const override {
    return sStorageVersion;
  }

  void migrateIfNecessary();

  StorageBookmarkCategory addBookmarkCategory(const StorageBookmarkCategoryData& data);
  StorageBookmark addBookmark(const StorageBookmarkData& data);
  StorageBookmarkedNode addBookmarkedNode(const StorageBookmarkedNodeData& data);
  StorageBookmarkedEdge addBookmarkedEdge(const StorageBookmarkedEdgeData data);

  void removeBookmarkCategory(Id id);
  void removeBookmark(Id id);

  std::vector<StorageBookmark> getAllBookmarks() const;
  std::vector<StorageBookmarkedNode> getAllBookmarkedNodes() const;
  std::vector<StorageBookmarkedEdge> getAllBookmarkedEdges() const;

  void updateBookmark(Id bookmarkId, const std::wstring& name, const std::wstring& comment, Id categoryId);

  std::vector<StorageBookmarkCategory> getAllBookmarkCategories() const;
  StorageBookmarkCategory getBookmarkCategoryByName(const std::wstring& name) const;

private:
  static constexpr size_t sStorageVersion = 2;

  std::vector<std::pair<int, SqliteDatabaseIndex>> getIndices() const;
  void clearTables() override;
  void setupTables() override;
  void setupPrecompiledStatements() override;

  template <typename ResultType>
  [[nodiscard]] std::vector<ResultType> doGetAll(const std::string& query) const;

  template <typename ResultType>
  [[nodiscard]] ResultType doGetFirst(const std::string& query) const {
    if(const auto results = doGetAll<ResultType>(query + " LIMIT 1"); !results.empty()) {
      return results.front();
    }
    return {};
  }
};

template <>
std::vector<StorageBookmarkCategory> SqliteBookmarkStorage::doGetAll<StorageBookmarkCategory>(const std::string& query) const;
template <>
std::vector<StorageBookmark> SqliteBookmarkStorage::doGetAll<StorageBookmark>(const std::string& query) const;
template <>
std::vector<StorageBookmarkedNode> SqliteBookmarkStorage::doGetAll<StorageBookmarkedNode>(const std::string& query) const;
template <>
std::vector<StorageBookmarkedEdge> SqliteBookmarkStorage::doGetAll<StorageBookmarkedEdge>(const std::string& query) const;