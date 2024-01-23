#pragma once
// internal
#include "InterprocessIndexerCommandManager.h"
#include "InterprocessIndexingStatusManager.h"
#include "InterprocessIntermediateStorageManager.h"

class InterprocessIndexer final {
public:
  InterprocessIndexer(const std::string& uuid, Id processId);

  void work();

private:
  InterprocessIndexerCommandManager m_interprocessIndexerCommandManager;
  InterprocessIndexingStatusManager m_interprocessIndexingStatusManager;
  InterprocessIntermediateStorageManager m_interprocessIntermediateStorageManager;

  const std::string m_uuid;
  const Id m_processId;
};