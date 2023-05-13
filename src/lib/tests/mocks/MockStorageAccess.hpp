// internal
#include "StorageAccess.h"
#include "NodeTypeSet.h"
#include "FileInfo.h"
// trompeloeil
#include <trompeloeil.hpp>

class MockedStroageAccess : public StorageAccess {
public:
  using Ids = std::vector<Id>;
  using NameHierarchies = std::vector<NameHierarchy>;
  using MapIdToName = std::map<Id, std::pair<Id, NameHierarchy>>;
  using SourceLocationCollectionPtr = std::shared_ptr<SourceLocationCollection>;
  using SearchMatchs = std::vector<SearchMatch>;
  using GraphPtr = std::shared_ptr<Graph>;
  using TypeMask = Edge::TypeMask;
  using SourceLocationFilePtr = std::shared_ptr<SourceLocationFile>;
  using TextAccessPtr = std::shared_ptr<TextAccess>;
  using VecFileInfo = std::vector<FileInfo>;
  using VecErrorInfo = std::vector<ErrorInfo>;
  using NodeBookmarks = std::vector<NodeBookmark>;
  using EdgeBookmarks = std::vector<EdgeBookmark>;
  using BookmarkCategories = std::vector<BookmarkCategory>;
  using FilePaths = std::vector<FilePath>;

  ~MockedStroageAccess() override = default;

  MAKE_CONST_MOCK1(getNodeIdForFileNode, Id(const FilePath&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getNodeIdForNameHierarchy, Id(const NameHierarchy&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getNodeIdsForNameHierarchies, Ids(const NameHierarchies)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getNameHierarchyForNodeId, NameHierarchy(Id)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getNameHierarchiesForNodeIds, NameHierarchies(const Ids&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getNodeIdToParentFileMap, MapIdToName(const Ids&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getNodeTypeForNodeWithId, NodeType(Id)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getEdgeById, StorageEdge(Id)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK2(getFullTextSearchLocations, SourceLocationCollectionPtr(const std::wstring&, bool)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK3(getAutocompletionMatches, SearchMatchs(const std::wstring&, NodeTypeSet, bool)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getSearchMatchesForTokenIds, SearchMatchs(const Ids&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK0(getGraphForAll, GraphPtr()); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getGraphForNodeTypes, GraphPtr(NodeTypeSet)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK3(getGraphForActiveTokenIds, GraphPtr(const Ids&, const Ids&, bool*)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getGraphForChildrenOfNodeId, GraphPtr(Id)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK7(getGraphForTrail, GraphPtr(Id,Id,NodeKindMask, TypeMask, bool, size_t, bool)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK0(getAvailableNodeTypes, NodeKindMask()); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK0(getAvailableEdgeTypes, TypeMask()); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK2(getActiveTokenIdsForId, Ids(Id, Id*)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getNodeIdsForLocationIds, Ids(const Ids&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getSourceLocationsForTokenIds, SourceLocationCollectionPtr(const Ids&)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getSourceLocationsForLocationIds, SourceLocationCollectionPtr(const Ids&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getSourceLocationsForFile, SourceLocationFilePtr(const FilePath&)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK3(getSourceLocationsForLinesInFile, SourceLocationFilePtr(const FilePath&, size_t, size_t)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK2(getSourceLocationsOfTypeInFile, SourceLocationFilePtr(const FilePath&, LocationType)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK2(getFileContent, TextAccessPtr(const FilePath&, bool)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getFileInfoForFileId, FileInfo(Id)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK1(getFileInfoForFilePath, FileInfo(const FilePath&)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getFileInfosForFilePaths, VecFileInfo(const FilePaths&)); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK0(getStorageStats, StorageStats()); // NOLINT(readability-identifier-length)

  MAKE_CONST_MOCK0(getErrorCount, ErrorCountInfo()); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getErrorsLimited, VecErrorInfo(const ErrorFilter&)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK2(getErrorsForFileLimited, VecErrorInfo(const ErrorFilter&, const FilePath&)); // NOLINT(readability-identifier-length)
  MAKE_CONST_MOCK1(getErrorSourceLocations, SourceLocationCollectionPtr(const VecErrorInfo&)); // NOLINT(readability-identifier-length)

  MAKE_MOCK1(addNodeBookmark, Id(const NodeBookmark&)); // NOLINT(readability-identifier-length)
  MAKE_MOCK1(addEdgeBookmark, Id(const EdgeBookmark&)); // NOLINT(readability-identifier-length)
  MAKE_MOCK1(addBookmarkCategory, Id(const std::wstring&)); // NOLINT(readability-identifier-length)

  MAKE_MOCK4(updateBookmark, void(const Id, const std::wstring&, const std::wstring&, const std::wstring&)); // NOLINT(readability-identifier-length)
  MAKE_MOCK1(removeBookmark, void(const Id)); // NOLINT(readability-identifier-length)
  MAKE_MOCK1(removeBookmarkCategory, void(const Id)); // NOLINT(readability-identifier-length)


  MAKE_CONST_MOCK0(getAllNodeBookmarks, NodeBookmarks());
  MAKE_CONST_MOCK0(getAllEdgeBookmarks, EdgeBookmarks());
  MAKE_CONST_MOCK0(getAllBookmarkCategories, BookmarkCategories());

  MAKE_CONST_MOCK2(getTooltipInfoForTokenIds, TooltipInfo(const Ids&, TooltipOrigin));
  MAKE_CONST_MOCK2(getTooltipInfoForSourceLocationIdsAndLocalSymbolIds, TooltipInfo(const Ids&, const Ids&));

  MAKE_MOCK1(setUseErrorCache, void(bool));
  MAKE_MOCK2(addErrorsToCache, void(const VecErrorInfo&, const ErrorCountInfo&));
};