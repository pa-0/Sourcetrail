#include <gtest/gtest.h>

#include "IntermediateStorage.h"
#include "ParseLocation.h"
#include "PersistentStorage.h"
#include "utilityString.h"

namespace {
class TestStorage : public PersistentStorage {
public:
  TestStorage() : PersistentStorage(FilePath(L"data/test.sqlite"), FilePath(L"data/testBookmarks.sqlite")) {
    clear();
  }

  // const size_t getNodeCount() const
  //{
  //	return getGraph().getNodeCount();
  //}

  // const size_t getEdgeCount() const
  //{
  //	return getGraph().getEdgeCount();
  //}
};

[[maybe_unused]] ParseLocation validLocation(Id locationId = 0) {
  return ParseLocation(1, 1, locationId, 1, locationId);
}

NameHierarchy createNameHierarchy(std::wstring s) {
  NameHierarchy nameHierarchy(NAME_DELIMITER_CXX);
  for(std::wstring element : utility::splitToVector(s, nameDelimiterTypeToString(NAME_DELIMITER_CXX))) {
    nameHierarchy.push(element);
  }
  return nameHierarchy;
}

[[maybe_unused]] NameHierarchy createFunctionNameHierarchy(std::wstring ret, std::wstring name, std::wstring parameters) {
  NameHierarchy nameHierarchy = createNameHierarchy(name);
  std::wstring lastName = nameHierarchy.back().getName();
  nameHierarchy.pop();
  nameHierarchy.push(NameElement(lastName, ret, parameters));
  return nameHierarchy;
}
}    // namespace

TEST(Storage, savesFile) {
  TestStorage storage;

  std::wstring filePath = L"path/to/test.h";

  std::shared_ptr<IntermediateStorage> intermediateStorage = std::make_shared<IntermediateStorage>();
  Id id = intermediateStorage
              ->addNode(StorageNodeData(
                  nodeKindToInt(NODE_FILE), NameHierarchy::serialize(NameHierarchy(filePath, NAME_DELIMITER_FILE))))
              .first;
  intermediateStorage->addFile(StorageFile(id, filePath, L"someLanguage", "someTime", true, true));

  storage.inject(intermediateStorage.get());

  EXPECT_TRUE(storage.getNameHierarchyForNodeId(id).getQualifiedName() == filePath);
  EXPECT_TRUE(storage.getNodeTypeForNodeWithId(id).isFile());
}

TEST(Storage, savesNode) {
  NameHierarchy a = createNameHierarchy(L"type");

  TestStorage storage;

  std::shared_ptr<IntermediateStorage> intermediateStorage = std::make_shared<IntermediateStorage>();
  intermediateStorage->addNode(StorageNodeData(nodeKindToInt(NODE_TYPEDEF), NameHierarchy::serialize(a)));

  storage.inject(intermediateStorage.get());

  Id storedId = storage.getNodeIdForNameHierarchy(a);

  EXPECT_TRUE(storedId != 0);
  EXPECT_TRUE(storage.getNodeTypeForNodeWithId(storedId).getKind() == NODE_TYPEDEF);
}

TEST(Storage, savesFieldAsMember) {
  NameHierarchy a = createNameHierarchy(L"Struct");
  NameHierarchy b = createNameHierarchy(L"Struct::m_field");

  TestStorage storage;

  std::shared_ptr<IntermediateStorage> intermediateStorage = std::make_shared<IntermediateStorage>();

  Id aId = intermediateStorage->addNode(StorageNodeData(nodeKindToInt(NODE_STRUCT), NameHierarchy::serialize(a))).first;
  intermediateStorage->addSymbol(StorageSymbol(aId, DEFINITION_EXPLICIT));

  Id bId = intermediateStorage->addNode(StorageNodeData(nodeKindToInt(NODE_FIELD), NameHierarchy::serialize(b))).first;
  intermediateStorage->addSymbol(StorageSymbol(bId, DEFINITION_EXPLICIT));
  intermediateStorage->addEdge(StorageEdgeData(Edge::typeToInt(Edge::EDGE_MEMBER), aId, bId));

  storage.inject(intermediateStorage.get());
  bool foundEdge = false;

  const Id sourceId = storage.getNodeIdForNameHierarchy(a);
  const Id targetId = storage.getNodeIdForNameHierarchy(b);
  for(auto edge : storage.getStorageEdges()) {
    if(edge.sourceNodeId == sourceId && edge.targetNodeId == targetId && edge.type == Edge::typeToInt(Edge::EDGE_MEMBER)) {
      foundEdge = true;
    }
  }
  EXPECT_TRUE(foundEdge);
}

TEST(Storage, savesMethodStatic) {
  // TestStorage storage;
  // Id id = storage.onMethodParsed(
  //	validLocation(1),
  //	ParseFunction(typeUsage("void"), createNameHierarchy("isMethod"), parameters("bool"), true),
  //	ParserClient::ACCESS_NONE,
  //	ParserClient::ABSTRACTION_NONE,
  //	validLocation(4)
  //);

  // Node* node = storage.getNodeWithId(id);
  // TS_ASSERT(node);
  // TS_ASSERT_EQUALS(node->getQualifiedNameWithSignature(), "isMethod");
  // TS_ASSERT_EQUALS(node->getType(), NODE_METHOD);
  // TS_ASSERT(node->getComponent<TokenComponentStatic>());
}

TEST(Storage, clearsSingleFileDataOfSingleFileStorage) {
  /*
  m_filePath = FilePath(L"file.cpp");
  TestStorage storage;
  storage.onFunctionParsed(
    validLocation(), ParseFunction(typeUsage("bool"), createNameHierarchy("isTrue"),
    parameters("char")), validLocation()
  );

  EXPECT_TRUE(storage.getNodeCount() == 3);
  EXPECT_TRUE(storage.getEdgeCount() == 2);
  EXPECT_TRUE(storage.tokenLocationCollection().getTokenLocations().size() == 4);

  std::set<FilePath> files;
  files.insert(FilePath(m_filePath));
  storage.clearFileData(files);

  EXPECT_TRUE(storage.getNodeCount() == 0);
  EXPECT_TRUE(storage.getEdgeCount() == 0);
  EXPECT_TRUE(storage.tokenLocationCollection().getTokenLocations().size() == 0);;*/
}

TEST(Storage, clearsUnreferencedSingleFileDataOfMultiFileStorage) {
  // m_filePath = "file.h";

  // TestStorage storage;

  // ParseFunction isTrue = ParseFunction(typeUsage("bool"), createNameHierarchy("isTrue"),
  // parameters("char")); storage.onFunctionParsed(validLocation(), isTrue, validLocation());

  // m_filePath = "file.cpp";

  // ParseFunction main = ParseFunction(typeUsage("int"), createNameHierarchy("main"),
  // parameters("void")); storage.onFunctionParsed(validLocation(), main, validLocation());

  // storage.onCallParsed(validLocation(), main, isTrue);

  // TS_ASSERT_EQUALS(storage.getNodeCount(), 6);
  // TS_ASSERT_EQUALS(storage.getEdgeCount(), 5);
  // TS_ASSERT_EQUALS(storage.tokenLocationCollection().getTokenLocations().size(), 9);

  // std::set<FilePath> files;
  // files.insert(FilePath("file.cpp"));
  // storage.clearFileData(files);

  // TS_ASSERT_EQUALS(storage.getNodeCount(), 3);
  // TS_ASSERT_EQUALS(storage.getEdgeCount(), 2);
  // TS_ASSERT_EQUALS(storage.tokenLocationCollection().getTokenLocations().size(), 4);*/
}

TEST(Storage, clearsReferencedSingleFileDataOfMultiFileStorage) {
  // m_filePath = "file.h";

  // TestStorage storage;

  // ParseFunction isTrue = ParseFunction(typeUsage("bool"), createNameHierarchy("isTrue"),
  // parameters("void")); storage.onFunctionParsed(validLocation(), isTrue, validLocation());

  // m_filePath = "file.cpp";

  // ParseFunction main = ParseFunction(typeUsage("int"), createNameHierarchy("main"),
  // parameters("void")); storage.onFunctionParsed(validLocation(), main, validLocation());

  // storage.onCallParsed(validLocation(), main, isTrue);

  // TS_ASSERT_EQUALS(storage.getNodeCount(), 5);
  // TS_ASSERT_EQUALS(storage.getEdgeCount(), 5);
  // TS_ASSERT_EQUALS(storage.tokenLocationCollection().getTokenLocations().size(), 9);

  // std::set<FilePath> files;
  // files.insert(FilePath("file.h"));
  // storage.clearFileData(files);

  // TS_ASSERT_EQUALS(storage.getNodeCount(), 4);
  // TS_ASSERT_EQUALS(storage.getEdgeCount(), 3);
  // TS_ASSERT_EQUALS(storage.tokenLocationCollection().getTokenLocations().size(), 5);
}

TEST(Storage, clearsMultiFileDataOfMultiFileStorage) {
  // m_filePath = "file.h";

  // TestStorage storage;

  // ParseFunction isTrue = ParseFunction(typeUsage("bool"), createNameHierarchy("isTrue"),
  // parameters("void")); storage.onFunctionParsed(validLocation(), isTrue, validLocation());

  // m_filePath = "file.cpp";

  // ParseFunction main = ParseFunction(typeUsage("int"), createNameHierarchy("main"),
  // parameters("void")); storage.onFunctionParsed(validLocation(), main, validLocation());

  // storage.onCallParsed(validLocation(), main, isTrue);

  // TS_ASSERT_EQUALS(storage.getNodeCount(), 5);
  // TS_ASSERT_EQUALS(storage.getEdgeCount(), 5);
  // TS_ASSERT_EQUALS(storage.tokenLocationCollection().getTokenLocations().size(), 9);

  // std::set<FilePath> filePaths;
  // filePaths.insert(FilePath("file.cpp"));
  // filePaths.insert(FilePath("file.h"));
  // storage.clearFileData(filePaths);

  // TS_ASSERT_EQUALS(storage.getNodeCount(), 0);
  // TS_ASSERT_EQUALS(storage.getEdgeCount(), 0);
  // TS_ASSERT_EQUALS(storage.tokenLocationCollection().getTokenLocations().size(), 0);
}

TEST(Storage, findsAndRemovesDependingFileNodes) {
  // TestStorage storage;

  // Id id1 = storage.onFileParsed("f.h");
  // Id id2 = storage.onFileParsed("file.h");
  // Id id3 = storage.onFileParsed("file.cpp");
  // Id id4 = storage.onFileIncludeParsed(validLocation(), "file.h", "f.h");
  // Id id5 = storage.onFileIncludeParsed(validLocation(), "file.cpp", "file.h");

  // std::string name1 = storage.getNodeWithId(id2)->getQualifiedNameWithSignature();
  // std::string name2 = storage.getNodeWithId(id3)->getQualifiedNameWithSignature();

  // std::set<FilePath> filePaths;
  // filePaths.insert(FilePath(name1));
  // std::set<FilePath> dependingFilePaths = storage.getDependingFilePathsAndRemoveFileNodes(filePaths);

  // TS_ASSERT_EQUALS(dependingFilePaths.size(), 1);
  // TS_ASSERT_EQUALS(dependingFilePaths.begin()->str(), name2);

  // TS_ASSERT(storage.getNodeWithId(id1));
  // TS_ASSERT(!storage.getNodeWithId(id2));
  // TS_ASSERT(!storage.getNodeWithId(id3));
  // TS_ASSERT(!storage.getEdgeWithId(id4));
  // TS_ASSERT(!storage.getEdgeWithId(id5));
}
