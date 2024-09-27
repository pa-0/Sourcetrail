#include <filesystem>
#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <system_error>

#include "SqliteBookmarkStorage.h"
#include "TimeStamp.h"

struct SqliteBookmarkStorageFix : testing::Test {
  void SetUp() override {
    mDatabase = std::make_unique<SqliteBookmarkStorage>(FilePath{sDatabasePath.data()});
    mDatabase->setup();
  }

  void TearDown() override {
    mDatabase.reset();
    std::error_code errorCode;
    EXPECT_TRUE(std::filesystem::remove(sDatabasePath, errorCode)) << errorCode.message();
  }

  std::unique_ptr<SqliteBookmarkStorage> mDatabase;
  static constexpr std::wstring_view sDatabasePath = L"data/SQLiteTestSuite/bookmarkTest.sqlite";
};

TEST_F(SqliteBookmarkStorageFix, addBookmarks) {
  // Given: the database is created.
  ASSERT_THAT(mDatabase, testing::NotNull());

  // When: 4 bookmarks are added.
  constexpr size_t BookmarkCount = 4;
  const Id categoryId = mDatabase->addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
  for(size_t i = 0; i < BookmarkCount; i++) {
    mDatabase->addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId));
  }

  // Then: The stored bookmark match the added ones.
  EXPECT_EQ(mDatabase->getAllBookmarks().size(), BookmarkCount);
}

TEST_F(SqliteBookmarkStorageFix, addBookmarkedNode) {
  // Given: the database is created.
  ASSERT_THAT(mDatabase, testing::NotNull());

  // When: 4 bookmark nodes are added.
  constexpr size_t BookmarkCount = 4;
  const Id bookmarkId = [this]() {
    const Id categoryId = mDatabase->addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
    return mDatabase->addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId))
                    .
                    id;
  }();
  for(size_t i = 0; i < BookmarkCount; i++) {
    mDatabase->addBookmarkedNode(StorageBookmarkedNodeData(bookmarkId, L"test name"));
  }

  // Then: The stored bookmark match the added ones.
  EXPECT_EQ(mDatabase->getAllBookmarkedNodes().size(), BookmarkCount);
}

TEST_F(SqliteBookmarkStorageFix, removeBookmarkAlsoRemovesBookmarkedNode) {
  // Given: the database is created.
  ASSERT_THAT(mDatabase, testing::NotNull());
  // And: A bookmark with node exists
  const auto bookmarkId = [this]() {
    const Id categoryId = mDatabase->addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
    const Id bookmarkId =
        mDatabase->addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId)).
                   id;
    mDatabase->addBookmarkedNode(StorageBookmarkedNodeData(bookmarkId, L"test name"));
    return bookmarkId;
  }();

  // When: The bookmark is removed
  mDatabase->removeBookmark(bookmarkId);

  // Then: No bookmark exists.
  EXPECT_THAT(mDatabase->getAllBookmarkedNodes(), testing::IsEmpty());
}

TEST_F(SqliteBookmarkStorageFix, editNodeBookmark) {
  // Given: the database is created.
  ASSERT_THAT(mDatabase, testing::NotNull());

  // And: A bookmark exists.
  const auto [bookmarkId, categoryId] = [this]() {
    const Id categoryId = mDatabase->addBookmarkCategory(StorageBookmarkCategoryData(L"test category")).id;
    const Id bookmarkId =
        mDatabase->addBookmark(StorageBookmarkData(L"test bookmark", L"test comment", TimeStamp::now().toString(), categoryId)).
                   id;
    mDatabase->addBookmarkedNode(StorageBookmarkedNodeData(bookmarkId, L"test name"));
    return std::pair{bookmarkId, categoryId};
  }();

  // When: Update the bookmark.
  const std::wstring updatedName = L"updated name";
  const std::wstring updatedComment = L"updated comment";
  mDatabase->updateBookmark(bookmarkId, updatedName, updatedComment, categoryId);
  const auto storageBookmark = mDatabase->getAllBookmarks().front();

  // Then: The bookmark match the updated values.
  EXPECT_EQ(updatedName, storageBookmark.name);
  EXPECT_EQ(updatedComment, storageBookmark.comment);
}