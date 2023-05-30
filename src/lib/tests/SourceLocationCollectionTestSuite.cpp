// Catch2
#include <catch2/catch_all.hpp>
// internal
#include "SourceLocation.h"
#include "SourceLocationCollection.h"
#include "SourceLocationFile.h"

// NOLINTNEXTLINE(cert-err58-cpp, readability-function-cognitive-complexity)
TEST_CASE("source locations get created with other end", "[lib]") {
  SourceLocationCollection collection;
  const auto* pLocation0 = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);

  REQUIRE_FALSE(pLocation0 == nullptr);
  REQUIRE(pLocation0->isStartLocation());
  REQUIRE(!pLocation0->isEndLocation());

  const auto* pLocation1 = pLocation0->getOtherLocation();

  REQUIRE_FALSE(pLocation1 == nullptr);
  REQUIRE(!pLocation1->isStartLocation());
  REQUIRE(pLocation1->isEndLocation());

  REQUIRE(pLocation0 == pLocation1->getOtherLocation());
  REQUIRE(pLocation0 == pLocation1->getStartLocation());
  REQUIRE(pLocation0 == pLocation0->getStartLocation());
  REQUIRE(pLocation1 == pLocation0->getEndLocation());
  REQUIRE(pLocation1 == pLocation1->getEndLocation());
}

TEST_CASE("source locations do not get created with wrong input", "[lib]") {
  SourceLocationCollection collection;
  auto* pLocation0 = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 2, 1);
  auto* pLocation1 = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {1}, FilePath(L"file.c"), 4, 1, 1, 10);

  REQUIRE(!pLocation0);
  REQUIRE(!pLocation1);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("source locations get unique id but both ends have the same", "[lib]") {
  SourceLocationCollection collection;
  SourceLocation* a = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 1, 1, 1, 1);
  SourceLocation* b = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {2}, FilePath(L"file.c"), 1, 1, 1, 1);
  SourceLocation* c = collection.addSourceLocation(
      LOCATION_TOKEN, 3, {3}, FilePath(L"file.c"), 1, 1, 1, 1);

  REQUIRE(1 == collection.getSourceLocationFileCount());
  REQUIRE(3 == collection.getSourceLocationCount());

  REQUIRE(a->getLocationId() == 1);
  REQUIRE(b->getLocationId() == 2);
  REQUIRE(c->getLocationId() == 3);

  REQUIRE(a->getLocationId() != b->getLocationId());
  REQUIRE(b->getLocationId() != c->getLocationId());
  REQUIRE(c->getLocationId() != a->getLocationId());

  REQUIRE(a->getLocationId() == a->getOtherLocation()->getLocationId());
  REQUIRE(b->getLocationId() == b->getOtherLocation()->getLocationId());
  REQUIRE(c->getLocationId() == c->getOtherLocation()->getLocationId());
}

TEST_CASE("source locations have right file path line column and token id", "[lib]") {
  SourceLocationCollection collection;
  auto* pLocation = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);

  REQUIRE(1 == pLocation->getTokenIds()[0]);
  REQUIRE(2 == pLocation->getLineNumber());
  REQUIRE(3 == pLocation->getColumnNumber());
  REQUIRE(4 == pLocation->getOtherLocation()->getLineNumber());
  REQUIRE(5 == pLocation->getOtherLocation()->getColumnNumber());
  REQUIRE(L"file.c" == pLocation->getFilePath().wstr());
}

TEST_CASE("finding source locations by id", "[lib]") {
  SourceLocationCollection collection;
  const auto* pLocation0 = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);
  const auto* pLocation1 = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {6}, FilePath(L"file.c"), 7, 8, 9, 10);

  REQUIRE(pLocation0 == collection.getSourceLocationById(pLocation0->getLocationId()));
  REQUIRE(pLocation1 == collection.getSourceLocationById(pLocation1->getLocationId()));
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("creating plain copy of all locations in line range", "[lib]") {
  SourceLocationCollection collection;
  SourceLocation* a = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 2, 3, 4, 5);
  SourceLocation* b = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {1}, FilePath(L"file.c"), 3, 3, 4, 5);
  SourceLocation* c = collection.addSourceLocation(
      LOCATION_TOKEN, 3, {1}, FilePath(L"file.c"), 1, 3, 5, 5);
  SourceLocation* d = collection.addSourceLocation(
      LOCATION_TOKEN, 4, {1}, FilePath(L"file.c"), 1, 5, 4, 5);

  Id ida = a->getLocationId();
  Id idb = b->getLocationId();
  Id idc = c->getLocationId();
  Id idd = d->getLocationId();

  unsigned int fromLine = 2;
  unsigned int toLine = 4;

  SourceLocationCollection copy;
  SourceLocation* x = collection.getSourceLocationById(ida);

  x->getSourceLocationFile()->forEachSourceLocation(
      [&copy, fromLine, toLine](SourceLocation* location) {
        if(location->getLineNumber() >= fromLine && location->getLineNumber() <= toLine) {
          copy.addSourceLocationCopy(location);
        }
      });

  REQUIRE(1 == copy.getSourceLocationFileCount());
  REQUIRE(3 == copy.getSourceLocationCount());

  REQUIRE(copy.getSourceLocationById(ida));
  REQUIRE(copy.getSourceLocationById(idb));
  REQUIRE(!copy.getSourceLocationById(idc));
  REQUIRE(copy.getSourceLocationById(idd));

  REQUIRE(a != copy.getSourceLocationById(ida));
  REQUIRE(d != copy.getSourceLocationById(idd));

  REQUIRE(copy.getSourceLocationById(ida)->getStartLocation());
  REQUIRE(copy.getSourceLocationById(ida)->getEndLocation());

  REQUIRE(!copy.getSourceLocationById(idd)->getStartLocation());
  REQUIRE(copy.getSourceLocationById(idd)->getEndLocation());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("get source locations filtered by lines", "[lib]") {
  SourceLocationCollection collection;
  SourceLocation* a = collection.addSourceLocation(
      LOCATION_TOKEN, 1, {1}, FilePath(L"file.c"), 1, 3, 1, 5);
  SourceLocation* b = collection.addSourceLocation(
      LOCATION_TOKEN, 2, {1}, FilePath(L"file.c"), 1, 3, 2, 5);
  SourceLocation* c = collection.addSourceLocation(
      LOCATION_TOKEN, 3, {1}, FilePath(L"file.c"), 2, 3, 2, 5);
  SourceLocation* d = collection.addSourceLocation(
      LOCATION_TOKEN, 4, {1}, FilePath(L"file.c"), 3, 3, 4, 5);
  SourceLocation* e = collection.addSourceLocation(
      LOCATION_TOKEN, 5, {1}, FilePath(L"file.c"), 3, 5, 5, 5);
  SourceLocation* f = collection.addSourceLocation(
      LOCATION_TOKEN, 6, {1}, FilePath(L"file.c"), 1, 5, 5, 5);
  SourceLocation* g = collection.addSourceLocation(
      LOCATION_TOKEN, 7, {1}, FilePath(L"file.c"), 5, 5, 5, 5);

  SourceLocationCollection copy;
  copy.addSourceLocationFile(collection.getSourceLocationById(a->getLocationId())
                                 ->getSourceLocationFile()
                                 ->getFilteredByLines(2, 4));

  REQUIRE(1 == copy.getSourceLocationFileCount());
  REQUIRE(4 == copy.getSourceLocationCount());

  REQUIRE(!copy.getSourceLocationById(a->getLocationId()));
  REQUIRE(copy.getSourceLocationById(b->getLocationId()));
  REQUIRE(copy.getSourceLocationById(c->getLocationId()));
  REQUIRE(copy.getSourceLocationById(d->getLocationId()));
  REQUIRE(copy.getSourceLocationById(e->getLocationId()));
  REQUIRE(!copy.getSourceLocationById(f->getLocationId()));
  REQUIRE(!copy.getSourceLocationById(g->getLocationId()));

  REQUIRE(b != copy.getSourceLocationById(b->getLocationId()));
  REQUIRE(c != copy.getSourceLocationById(c->getLocationId()));

  REQUIRE(!copy.getSourceLocationById(b->getLocationId())->getStartLocation());
  REQUIRE(copy.getSourceLocationById(b->getLocationId())->getEndLocation());

  REQUIRE(copy.getSourceLocationById(e->getLocationId())->getStartLocation());
  REQUIRE(!copy.getSourceLocationById(e->getLocationId())->getEndLocation());
}