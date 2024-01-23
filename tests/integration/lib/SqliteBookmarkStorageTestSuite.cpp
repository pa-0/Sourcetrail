#include <gtest/gtest.h>

#include "FileSystem.h"
#include "SqliteBookmarkStorage.h"

TEST(SqliteBookmarkStorage, addBookmarks) {
  FilePath databasePath(L"data/SQLiteTestSuite/bookmarkTest.sqlite");
  size_t bookmarkCount = 4;
  int result = -1;
  {
    FileSystem::remove(databasePath);
    SqliteBookmarkStorage storage(databasePath);
    storage.setup();

    for(size_t i = 0; i < bookmarkCount; i++) {
      const Id categoryId = storage.addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
      storage.addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId));
    }

    result = static_cast<int>(storage.getAllBookmarks().size());
  }

  FileSystem::remove(databasePath);

  EXPECT_TRUE(result == bookmarkCount);
}

TEST(SqliteBookmarkStorage, addBookmarkedNode) {
  FilePath databasePath(L"data/SQLiteTestSuite/bookmarkTest.sqlite");
  size_t bookmarkCount = 4;
  int result = -1;
  {
    FileSystem::remove(databasePath);
    SqliteBookmarkStorage storage(databasePath);
    storage.setup();

    const Id categoryId = storage.addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
    const Id bookmarkId =
        storage.addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId)).id;

    for(size_t i = 0; i < bookmarkCount; i++) {
      storage.addBookmarkedNode(StorageBookmarkedNodeData(bookmarkId, L"test name"));
    }

    result = static_cast<int>(storage.getAllBookmarkedNodes().size());
  }

  FileSystem::remove(databasePath);

  EXPECT_TRUE(result == bookmarkCount);
}

TEST(SqliteBookmarkStorage, removeBookmarkAlsoRemovesBookmarkedNode) {
  FilePath databasePath(L"data/SQLiteTestSuite/bookmarkTest.sqlite");
  int result = -1;
  {
    FileSystem::remove(databasePath);
    SqliteBookmarkStorage storage(databasePath);
    storage.setup();

    const Id categoryId = storage.addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
    const Id bookmarkId =
        storage.addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId)).id;
    storage.addBookmarkedNode(StorageBookmarkedNodeData(bookmarkId, L"test name"));

    storage.removeBookmark(bookmarkId);

    result = static_cast<int>(storage.getAllBookmarkedNodes().size());
  }

  FileSystem::remove(databasePath);

  EXPECT_TRUE(result == 0);
}

TEST(SqliteBookmarkStorage, editNodeBookmark) {
  FilePath databasePath(L"data/SQLiteTestSuite/bookmarkTest.sqlite");

  const std::wstring updatedName = L"updated name";
  const std::wstring updatedComment = L"updated comment";

  StorageBookmark storageBookmark;
  {
    FileSystem::remove(databasePath);
    SqliteBookmarkStorage storage(databasePath);
    storage.setup();

    const Id categoryId = storage.addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
    const Id bookmarkId =
        storage.addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId)).id;
    storage.addBookmarkedNode(StorageBookmarkedNodeData(bookmarkId, L"test name"));

    storage.updateBookmark(bookmarkId, updatedName, updatedComment, categoryId);

    storageBookmark = storage.getAllBookmarks().front();
  }

  FileSystem::remove(databasePath);

  EXPECT_TRUE(updatedName == storageBookmark.name);
  EXPECT_TRUE(updatedComment == storageBookmark.comment);
}
