#include <gtest/gtest.h>

#include "FileSystem.h"
#include "SqliteIndexStorage.h"

TEST(SqliteIndexStorage, addsNodeSuccessfully) {
  FilePath databasePath(L"data/SQLiteTestSuite/test.sqlite");
  int nodeCount = -1;
  {
    SqliteIndexStorage storage(databasePath);
    storage.setup();
    storage.beginTransaction();
    storage.addNode(StorageNodeData(0, L"a"));
    storage.commitTransaction();
    nodeCount = storage.getNodeCount();
  }
  FileSystem::remove(databasePath);

  EXPECT_TRUE(1 == nodeCount);
}

TEST(SqliteIndexStorage, removesNodeSuccessfully) {
  FilePath databasePath(L"data/SQLiteTestSuite/test.sqlite");
  int nodeCount = -1;
  {
    SqliteIndexStorage storage(databasePath);
    storage.setup();
    storage.beginTransaction();
    Id nodeId = storage.addNode(StorageNodeData(0, L"a"));
    storage.removeElement(nodeId);
    storage.commitTransaction();
    nodeCount = storage.getNodeCount();
  }
  FileSystem::remove(databasePath);

  EXPECT_TRUE(0 == nodeCount);
}

TEST(SqliteIndexStorage, addsEdgeSuccessfully) {
  FilePath databasePath(L"data/SQLiteTestSuite/test.sqlite");
  int edgeCount = -1;
  {
    SqliteIndexStorage storage(databasePath);
    storage.setup();
    storage.beginTransaction();
    Id sourceNodeId = storage.addNode(StorageNodeData(0, L"a"));
    Id targetNodeId = storage.addNode(StorageNodeData(0, L"b"));
    storage.addEdge(StorageEdgeData(0, sourceNodeId, targetNodeId));
    storage.commitTransaction();
    edgeCount = storage.getEdgeCount();
  }
  FileSystem::remove(databasePath);

  EXPECT_TRUE(1 == edgeCount);
}

TEST(SqliteIndexStorage, removesEdgeSuccessfully) {
  FilePath databasePath(L"data/SQLiteTestSuite/test.sqlite");
  int edgeCount = -1;
  {
    SqliteIndexStorage storage(databasePath);
    storage.setup();
    storage.beginTransaction();
    Id sourceNodeId = storage.addNode(StorageNodeData(0, L"a"));
    Id targetNodeId = storage.addNode(StorageNodeData(0, L"b"));
    Id edgeId = storage.addEdge(StorageEdgeData(0, sourceNodeId, targetNodeId));
    storage.removeElement(edgeId);
    storage.commitTransaction();
    edgeCount = storage.getEdgeCount();
  }
  FileSystem::remove(databasePath);

  EXPECT_TRUE(0 == edgeCount);
}