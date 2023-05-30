// GTest
#include <gmock/gmock.h>
#include <gtest/gtest.h>
// internal
#include "SourceLocation.h"
#include "SourceLocationCollection.h"
#include "SourceLocationFile.h"

// NOLINTNEXTLINE
TEST(SourceLocationCollection, SourceLocationsGetCreatedWithOtherEnd) {
  SourceLocationCollection collection;
  const auto* pLocation0 = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);

  EXPECT_NE(nullptr, pLocation0);
  EXPECT_TRUE(pLocation0->isStartLocation());
  EXPECT_FALSE(pLocation0->isEndLocation());

  const auto* pLocation1 = pLocation0->getOtherLocation();

  EXPECT_NE(nullptr, pLocation1);
  EXPECT_TRUE(!pLocation1->isStartLocation());
  EXPECT_TRUE(pLocation1->isEndLocation());

  EXPECT_EQ(pLocation0, pLocation1->getOtherLocation());
  EXPECT_EQ(pLocation0, pLocation1->getStartLocation());
  EXPECT_EQ(pLocation0, pLocation0->getStartLocation());
  EXPECT_EQ(pLocation1, pLocation0->getEndLocation());
  EXPECT_EQ(pLocation1, pLocation1->getEndLocation());
}

// NOLINTNEXTLINE
TEST(SourceLocationCollection, SourceLocationsDoNotGetCreatedWithWrongInput) {
  SourceLocationCollection collection;
  auto* pLocation0 = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 2, 1);
  auto* pLocation1 = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {1}, FilePath(L"file.c"), 4, 1, 1, 10);

  EXPECT_EQ(nullptr, pLocation0);
  EXPECT_EQ(nullptr, pLocation1);
}

// NOLINTNEXTLINE
TEST(SourceLocationCollection, SourceLocationsGetUniqueIdButBothEndsHaveTheSame) {
  SourceLocationCollection collection;
  auto* a = collection.addSourceLocation(LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 1, 1, 1, 1);
  auto* b = collection.addSourceLocation(LOCATION_TOKEN, 2, {2}, FilePath(L"file.c"), 1, 1, 1, 1);
  auto* c = collection.addSourceLocation(LOCATION_TOKEN, 3, {3}, FilePath(L"file.c"), 1, 1, 1, 1);

  EXPECT_EQ(1, collection.getSourceLocationFileCount());
  EXPECT_EQ(3, collection.getSourceLocationCount());

  EXPECT_EQ(1, a->getLocationId());
  EXPECT_EQ(2, b->getLocationId());
  EXPECT_EQ(3, c->getLocationId());

  EXPECT_NE(a->getLocationId(), b->getLocationId());
  EXPECT_NE(b->getLocationId(), c->getLocationId());
  EXPECT_NE(c->getLocationId(), a->getLocationId());

  EXPECT_EQ(a->getLocationId(), a->getOtherLocation()->getLocationId());
  EXPECT_EQ(b->getLocationId(), b->getOtherLocation()->getLocationId());
  EXPECT_EQ(c->getLocationId(), c->getOtherLocation()->getLocationId());
}

// NOLINTNEXTLINE
TEST(SourceLocationCollection, SourceLocationsHaveRightFilePathLineColumnAndTokenId) {
  SourceLocationCollection collection;
  auto* pLocation = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);

  EXPECT_EQ(1, pLocation->getTokenIds()[0]);
  EXPECT_EQ(2, pLocation->getLineNumber());
  EXPECT_EQ(3, pLocation->getColumnNumber());
  EXPECT_EQ(4, pLocation->getOtherLocation()->getLineNumber());
  EXPECT_EQ(5, pLocation->getOtherLocation()->getColumnNumber());
  EXPECT_THAT(pLocation->getFilePath().wstr(), testing::StrEq(L"file.c"));
}

// NOLINTNEXTLINE
TEST(SourceLocationCollection, FindingSourceLocationsById) {
  SourceLocationCollection collection;
  const auto* pLocation0 = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);
  const auto* pLocation1 = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {6}, FilePath(L"file.c"), 7, 8, 9, 10);

  EXPECT_EQ(pLocation0, collection.getSourceLocationById(pLocation0->getLocationId()));
  EXPECT_EQ(pLocation1, collection.getSourceLocationById(pLocation1->getLocationId()));
}

// NOLINTNEXTLINE
TEST(SourceLocationCollection, CreatingPlainCopyOfAllLocationsInLineRange) {
  const FilePath filePath {L"file.c"};
  SourceLocationCollection collection;
  auto* a = collection.addSourceLocation(LOCATION_TOKEN, 1, {1}, filePath, 2, 3, 4, 5);
  auto* b = collection.addSourceLocation(LOCATION_TOKEN, 2, {1}, filePath, 3, 3, 4, 5);
  auto* c = collection.addSourceLocation(LOCATION_TOKEN, 3, {1}, filePath, 1, 3, 5, 5);
  auto* d = collection.addSourceLocation(LOCATION_TOKEN, 4, {1}, filePath, 1, 5, 4, 5);

  Id ida = a->getLocationId();
  Id idb = b->getLocationId();
  Id idc = c->getLocationId();
  Id idd = d->getLocationId();

  unsigned int fromLine = 2;
  unsigned int toLine = 4;

  SourceLocationCollection copy;
  auto* x = collection.getSourceLocationById(ida);

  x->getSourceLocationFile()->forEachSourceLocation(
      [&copy, fromLine, toLine](SourceLocation* location) {
        if(location->getLineNumber() >= fromLine && location->getLineNumber() <= toLine) {
          copy.addSourceLocationCopy(location);
        }
      });

  EXPECT_EQ(1, copy.getSourceLocationFileCount());
  EXPECT_EQ(3, copy.getSourceLocationCount());

  EXPECT_TRUE(copy.getSourceLocationById(ida));
  EXPECT_TRUE(copy.getSourceLocationById(idb));
  EXPECT_FALSE(copy.getSourceLocationById(idc));
  EXPECT_TRUE(copy.getSourceLocationById(idd));

  EXPECT_NE(a, copy.getSourceLocationById(ida));
  EXPECT_NE(d, copy.getSourceLocationById(idd));

  EXPECT_TRUE(copy.getSourceLocationById(ida)->getStartLocation());
  EXPECT_TRUE(copy.getSourceLocationById(ida)->getEndLocation());

  EXPECT_FALSE(copy.getSourceLocationById(idd)->getStartLocation());
  EXPECT_TRUE(copy.getSourceLocationById(idd)->getEndLocation());
}

// NOLINTNEXTLINE
TEST(Sourcelocationcollection, GetSourceLocationsFilteredByLines) {
  const FilePath filePath {L"file.c"};
  SourceLocationCollection collection;
  auto* a = collection.addSourceLocation(LOCATION_TOKEN, 1, {1}, filePath, 1, 3, 1, 5);
  auto* b = collection.addSourceLocation(LOCATION_TOKEN, 2, {1}, filePath, 1, 3, 2, 5);
  auto* c = collection.addSourceLocation(LOCATION_TOKEN, 3, {1}, filePath, 2, 3, 2, 5);
  auto* d = collection.addSourceLocation(LOCATION_TOKEN, 4, {1}, filePath, 3, 3, 4, 5);
  auto* e = collection.addSourceLocation(LOCATION_TOKEN, 5, {1}, filePath, 3, 5, 5, 5);
  auto* f = collection.addSourceLocation(LOCATION_TOKEN, 6, {1}, filePath, 1, 5, 5, 5);
  auto* g = collection.addSourceLocation(LOCATION_TOKEN, 7, {1}, filePath, 5, 5, 5, 5);

  SourceLocationCollection copy;
  copy.addSourceLocationFile(collection.getSourceLocationById(a->getLocationId())
                                 ->getSourceLocationFile()
                                 ->getFilteredByLines(2, 4));

  EXPECT_EQ(1, copy.getSourceLocationFileCount());
  EXPECT_EQ(4, copy.getSourceLocationCount());

  EXPECT_FALSE(copy.getSourceLocationById(a->getLocationId()));
  EXPECT_TRUE(copy.getSourceLocationById(b->getLocationId()));
  EXPECT_TRUE(copy.getSourceLocationById(c->getLocationId()));
  EXPECT_TRUE(copy.getSourceLocationById(d->getLocationId()));
  EXPECT_TRUE(copy.getSourceLocationById(e->getLocationId()));
  EXPECT_FALSE(copy.getSourceLocationById(f->getLocationId()));
  EXPECT_FALSE(copy.getSourceLocationById(g->getLocationId()));

  EXPECT_NE(b, copy.getSourceLocationById(b->getLocationId()));
  EXPECT_NE(c, copy.getSourceLocationById(c->getLocationId()));

  EXPECT_FALSE(copy.getSourceLocationById(b->getLocationId())->getStartLocation());
  EXPECT_TRUE(copy.getSourceLocationById(b->getLocationId())->getEndLocation());

  EXPECT_TRUE(copy.getSourceLocationById(e->getLocationId())->getStartLocation());
  EXPECT_FALSE(copy.getSourceLocationById(e->getLocationId())->getEndLocation());
}