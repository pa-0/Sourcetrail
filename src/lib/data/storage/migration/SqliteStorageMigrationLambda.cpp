#include "SqliteStorageMigrationLambda.h"


SqliteStorageMigrationLambda::SqliteStorageMigrationLambda(std::function<void(const SqliteStorageMigration*, SqliteStorage*)> lambda)
    : m_lambda(std::move(lambda)) {}

SqliteStorageMigrationLambda::~SqliteStorageMigrationLambda() {}

void SqliteStorageMigrationLambda::apply(SqliteStorage* migratable) const {
  m_lambda(this, migratable);
}
