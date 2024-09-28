#include "SqliteBookmarkStorage.h"

#include <fmt/format.h>

#include "Application.h"
#include "ProjectSettings.h"
#include "SqliteStorageMigrationLambda.h"
#include "SqliteStorageMigrator.h"
#include "logging.h"
#include "utilityString.h"

SqliteBookmarkStorage::SqliteBookmarkStorage(const FilePath& dbFilePath)
  : SqliteStorage(dbFilePath) {
}

void SqliteBookmarkStorage::migrateIfNecessary() {
  SqliteStorageMigrator migrator;

  migrator.addMigration(
      2,
      std::make_shared<SqliteStorageMigrationLambda>([](const SqliteStorageMigration* migration, SqliteStorage* storage) {
        if(Application::getInstance()) {
          {
            // TODO(Hussein): Need to check why it's needed?
            const auto currentProject = Application::getInstance()->getCurrentProject();
            [[maybe_unused]] LanguageType currentLanguage = ProjectSettings::getLanguageOfProject(
                currentProject->getProjectSettingsFilePath());
          }
        }
        constexpr std::string_view separator = "::";
        migration->executeStatementInStorage(
            storage,
            fmt::format("UPDATE bookmarked_node SET serialized_node_name = '{}\tm' || serialized_node_name", separator));
        migration->executeStatementInStorage(
            storage,
            fmt::format("UPDATE bookmarked_edge SET serialized_source_node_name = '{}\tm' || serialized_source_node_name",
                        separator));
        migration->executeStatementInStorage(
            storage,
            fmt::format("UPDATE bookmarked_edge SET serialized_target_node_name = '{}\tm' || serialized_target_node_name",
                        separator));
      }));

  migrator.migrate(this, SqliteBookmarkStorage::sStorageVersion);
}

StorageBookmarkCategory SqliteBookmarkStorage::addBookmarkCategory(const StorageBookmarkCategoryData& data) {
  constexpr std::string_view statement =
      "INSERT INTO bookmark_category(id, name) "
      "VALUES (NULL, ?);";

  CppSQLite3Statement stmt = m_database.compileStatement(statement.data());
  stmt.bind(1, utility::encodeToUtf8(data.name).c_str());

  if(!executeStatement(stmt)) {
    LOG_WARNING("Failed to addBookmarkCategory");
    return {static_cast<Id>(-1), StorageBookmarkCategoryData{}};
  }
  return {static_cast<Id>(m_database.lastRowId()), data};
}

StorageBookmark SqliteBookmarkStorage::addBookmark(const StorageBookmarkData& data) {
  std::string const statement = fmt::format(
      "INSERT INTO bookmark(id, name, comment, timestamp, category_id) "
      "VALUES (NULL, ?, ?, ?, {});",
      data.categoryId);

  try {
    CppSQLite3Statement stmt = m_database.compileStatement(statement.c_str());
    stmt.bind(1, utility::encodeToUtf8(data.name).c_str());
    stmt.bind(2, utility::encodeToUtf8(data.comment).c_str());
    stmt.bind(3, data.timestamp.c_str());
    executeStatement(stmt);

    return {static_cast<Id>(m_database.lastRowId()), data};
  } catch(CppSQLite3Exception& e) {
    LOG_ERROR(std::to_string(e.errorCode()) + ": " + e.errorMessage());
  }
  return {};
}

StorageBookmarkedNode SqliteBookmarkStorage::addBookmarkedNode(const StorageBookmarkedNodeData& data) {
  executeStatement("INSERT INTO bookmarked_element(id, bookmark_id) VALUES(NULL, " + std::to_string(data.bookmarkId) + ");");
  const Id lastRowId = static_cast<Id>(m_database.lastRowId());

  std::string statement =
      "INSERT INTO bookmarked_node(id, serialized_node_name) "
      "VALUES (" +
      std::to_string(lastRowId) + ", ?);";
  CppSQLite3Statement stmt = m_database.compileStatement(statement.c_str());
  stmt.bind(1, utility::encodeToUtf8(data.serializedNodeName).c_str());
  executeStatement(stmt);

  return {lastRowId, data};
}

StorageBookmarkedEdge SqliteBookmarkStorage::addBookmarkedEdge(const StorageBookmarkedEdgeData data) {
  executeStatement("INSERT INTO bookmarked_element(id, bookmark_id) VALUES(NULL, " + std::to_string(data.bookmarkId) + ");");
  const Id lastRowId = static_cast<Id>(m_database.lastRowId());

  std::string statement =
      "INSERT INTO bookmarked_edge(id, serialized_source_node_name, serialized_target_node_name, "
      "edge_type, source_node_active) "
      "VALUES (" +
      std::to_string(lastRowId) + ", ?, ?, " + std::to_string(data.edgeType) + ", " + std::to_string(data.sourceNodeActive) +
      ");";
  CppSQLite3Statement stmt = m_database.compileStatement(statement.c_str());
  stmt.bind(1, utility::encodeToUtf8(data.serializedSourceNodeName).c_str());
  stmt.bind(2, utility::encodeToUtf8(data.serializedTargetNodeName).c_str());
  executeStatement(stmt);

  return {lastRowId, data};
}

std::vector<StorageBookmark> SqliteBookmarkStorage::getAllBookmarks() const {
  return doGetAll<StorageBookmark>("");
}

void SqliteBookmarkStorage::removeBookmark(Id bookmarkId) {
  executeStatement("DELETE FROM bookmark WHERE id = (" + std::to_string(bookmarkId) + ");");
}

std::vector<StorageBookmarkedNode> SqliteBookmarkStorage::getAllBookmarkedNodes() const {
  return doGetAll<StorageBookmarkedNode>("");
}

std::vector<StorageBookmarkedEdge> SqliteBookmarkStorage::getAllBookmarkedEdges() const {
  return doGetAll<StorageBookmarkedEdge>("");
}

void SqliteBookmarkStorage::updateBookmark(const Id bookmarkId,
                                           const std::wstring& name,
                                           const std::wstring& comment,
                                           const Id categoryId) {
  executeStatement("UPDATE bookmark SET name = '" + utility::encodeToUtf8(name) + "' WHERE id == " + std::to_string(bookmarkId) +
      ";");
  executeStatement("UPDATE bookmark SET comment = '" + utility::encodeToUtf8(comment) +
      "' WHERE id == " + std::to_string(bookmarkId) + ";");
  executeStatement("UPDATE bookmark SET category_id = " + std::to_string(categoryId) +
      " WHERE id == " + std::to_string(bookmarkId) + ";");
}

std::vector<StorageBookmarkCategory> SqliteBookmarkStorage::getAllBookmarkCategories() const {
  return doGetAll<StorageBookmarkCategory>("");
}

StorageBookmarkCategory SqliteBookmarkStorage::getBookmarkCategoryByName(const std::wstring& name) const {
  return doGetFirst<StorageBookmarkCategory>(fmt::format("WHERE name == '{}'", utility::encodeToUtf8(name)));
}

void SqliteBookmarkStorage::removeBookmarkCategory(Id bookmarkCategoryId) {
  executeStatement("DELETE FROM bookmark_category WHERE id = (" + std::to_string(bookmarkCategoryId) + ");");
}

std::vector<std::pair<int, SqliteDatabaseIndex>> SqliteBookmarkStorage::getIndices() const {
  return {};
}

void SqliteBookmarkStorage::clearTables() {
  try {
    m_database.execDML("DROP TABLE IF EXISTS main.bookmarked_edge;");
    m_database.execDML("DROP TABLE IF EXISTS main.bookmarked_node;");
    m_database.execDML("DROP TABLE IF EXISTS main.bookmarked_element;");
    m_database.execDML("DROP TABLE IF EXISTS main.bookmark;");
    m_database.execDML("DROP TABLE IF EXISTS main.bookmark_category;");
  } catch(CppSQLite3Exception& e) {
    LOG_ERROR(std::to_string(e.errorCode()) + ": " + e.errorMessage());
  }
}

void SqliteBookmarkStorage::setupTables() {
  try {
    m_database.execDML(
        "CREATE TABLE IF NOT EXISTS bookmark_category("
        "id INTEGER NOT NULL, "
        "name TEXT, "
        "PRIMARY KEY(id)"
        ");");

    m_database.execDML(
        "CREATE TABLE IF NOT EXISTS bookmark("
        "id INTEGER NOT NULL, "
        "name TEXT, "
        "comment TEXT, "
        "timestamp TEXT, "
        "category_id INTEGER, "
        "FOREIGN KEY(category_id) REFERENCES bookmark_category(id) ON DELETE CASCADE, "
        "PRIMARY KEY(id)"
        ");");

    m_database.execDML(
        "CREATE TABLE IF NOT EXISTS bookmarked_element("
        "id INTEGER NOT NULL, "
        "bookmark_id INTEGER NOT NULL, "
        "FOREIGN KEY(bookmark_id) REFERENCES bookmark(id) ON DELETE CASCADE, "
        "PRIMARY KEY(id)"
        ");");

    m_database.execDML(
        "CREATE TABLE IF NOT EXISTS bookmarked_node("
        "id INTEGER NOT NULL, "
        "serialized_node_name TEXT, "
        "FOREIGN KEY(id) REFERENCES bookmarked_element(id) ON DELETE CASCADE, "
        "PRIMARY KEY(id)"
        ");");

    m_database.execDML(
        "CREATE TABLE IF NOT EXISTS bookmarked_edge("
        "id INTEGER NOT NULL, "
        "serialized_source_node_name TEXT, "
        "serialized_target_node_name TEXT, "
        "edge_type INTEGER, "
        "source_node_active INTEGER, "
        "FOREIGN KEY(id) REFERENCES bookmarked_element(id) ON DELETE CASCADE, "
        "PRIMARY KEY(id)"
        ");");
  } catch(CppSQLite3Exception& e) {
    LOG_ERROR(fmt::format("Failed to create tables: {}: {}", std::to_string(e.errorCode()), e.errorMessage()));
    throw e;
  } catch(std::exception& e) {
    LOG_ERROR(fmt::format("Failed to create tables: {}", e.what()));
    throw e;
  }
}

void SqliteBookmarkStorage::setupPrecompiledStatements() {
}

template <>
std::vector<StorageBookmarkCategory> SqliteBookmarkStorage::doGetAll<StorageBookmarkCategory>(const std::string& query) const {
  CppSQLite3Query queryStr = executeQuery("SELECT id, name FROM bookmark_category " + query + ";");

  std::vector<StorageBookmarkCategory> categories;
  while(!queryStr.eof()) {
    const Id id = queryStr.getIntField(0, 0);
    const std::string name = queryStr.getStringField(1, "");

    if(id != 0 && !name.empty()) {
      categories.emplace_back(id, utility::decodeFromUtf8(name));
    }

    queryStr.nextRow();
  }
  return categories;
}

template <>
std::vector<StorageBookmark> SqliteBookmarkStorage::doGetAll<StorageBookmark>(const std::string& queryString) const {
  CppSQLite3Query query = executeQuery("SELECT id, name, comment, timestamp, category_id FROM bookmark " + queryString + ";");

  std::vector<StorageBookmark> bookmarks;
  while(!query.eof()) {
    const Id bookmarksId = static_cast<Id>(query.getIntField(0, 0));
    const std::string name = query.getStringField(1, "");
    const std::string comment = query.getStringField(2, "");
    const std::string timestamp = query.getStringField(3, "");
    const Id categoryId = static_cast<Id>(query.getIntField(4, 0));

    if(bookmarksId != 0 && !name.empty() && !timestamp.empty()) {
      bookmarks.emplace_back(bookmarksId, utility::decodeFromUtf8(name), utility::decodeFromUtf8(comment), timestamp, categoryId);
    }

    query.nextRow();
  }
  return bookmarks;
}

template <>
std::vector<StorageBookmarkedNode> SqliteBookmarkStorage::doGetAll<StorageBookmarkedNode>(const std::string& queryString) const {
  CppSQLite3Query query = executeQuery(
      "SELECT "
      "bookmarked_node.id, bookmarked_element.bookmark_id, bookmarked_node.serialized_node_name "
      "FROM bookmarked_node "
      "INNER JOIN "
      "bookmarked_element ON bookmarked_node.id = bookmarked_element.id " +
      queryString + ";");

  std::vector<StorageBookmarkedNode> bookmarkedNodes;
  while(!query.eof()) {
    const Id nodeId = static_cast<Id>(query.getIntField(0, 0));
    const Id bookmarkId = static_cast<Id>(query.getIntField(1, 0));
    const std::string serializedNodeName = query.getStringField(2, "");

    if(nodeId != 0 && bookmarkId != 0 && !serializedNodeName.empty()) {
      bookmarkedNodes.emplace_back(nodeId, bookmarkId, utility::decodeFromUtf8(serializedNodeName));
    }

    query.nextRow();
  }
  return bookmarkedNodes;
}

template <>
std::vector<StorageBookmarkedEdge> SqliteBookmarkStorage::doGetAll<StorageBookmarkedEdge>(const std::string& queryString) const {
  CppSQLite3Query query = executeQuery(
      "SELECT "
      "bookmarked_edge.id, bookmarked_element.bookmark_id, "
      "bookmarked_edge.serialized_source_node_name, bookmarked_edge.serialized_target_node_name, "
      "bookmarked_edge.edge_type, bookmarked_edge.source_node_active "
      "FROM bookmarked_edge "
      "INNER JOIN "
      "bookmarked_element ON bookmarked_edge.id = bookmarked_element.id " +
      queryString + ";");

  std::vector<StorageBookmarkedEdge> bookmarkedEdges;
  while(!query.eof()) {
    const Id id = query.getIntField(0, 0);
    const Id bookmarkId = query.getIntField(1, 0);
    const std::string serializedSourceNodeName = query.getStringField(2, "");
    const std::string serializedTargetNodeName = query.getStringField(3, "");
    const int edgeType = query.getIntField(4, -1);
    const int sourceNodeActive = query.getIntField(5, -1);

    if(id != 0 && bookmarkId != 0 && !serializedSourceNodeName.empty() && !serializedTargetNodeName.empty() && edgeType != -1 &&
      sourceNodeActive != -1) {
      bookmarkedEdges.emplace_back(id,
                                   bookmarkId,
                                   utility::decodeFromUtf8(serializedSourceNodeName),
                                   utility::decodeFromUtf8(serializedTargetNodeName),
                                   edgeType,
                                   sourceNodeActive);
    }

    query.nextRow();
  }
  return bookmarkedEdges;
}