#include "InterprocessIndexingStatusManager.h"

#include "logging.h"
#include "utilityString.h"

const char* InterprocessIndexingStatusManager::sSharedMemoryNamePrefix = "ists_";

const char* InterprocessIndexingStatusManager::sIndexingFilesKeyName = "indexing_files";
const char* InterprocessIndexingStatusManager::sCurrentFilesKeyName = "current_files";
const char* InterprocessIndexingStatusManager::sCrashedFilesKeyName = "crashed_files";
const char* InterprocessIndexingStatusManager::sFinishedProcessIdsKeyName = "finished_process_ids";
const char* InterprocessIndexingStatusManager::sIndexingInterruptedKeyName = "indexing_interrupted_flag";

InterprocessIndexingStatusManager::InterprocessIndexingStatusManager(const std::string& instanceUuid, Id processId, bool isOwner)
    : BaseInterprocessDataManager(sSharedMemoryNamePrefix + instanceUuid, 1048576 /* 1 MB */, instanceUuid, processId, isOwner) {}

InterprocessIndexingStatusManager::~InterprocessIndexingStatusManager() = default;

void InterprocessIndexingStatusManager::startIndexingSourceFile(const FilePath& filePath) {
  SharedMemory::ScopedAccess access(&mSharedMemory);

  auto* indexingFilesPtr = access.accessValueWithAllocator<SharedMemory::Queue<SharedMemory::String>>(sIndexingFilesKeyName);
  if(indexingFilesPtr) {
    SharedMemory::String fileStr(access.getAllocator());
    fileStr = utility::encodeToUtf8(filePath.wstr()).c_str();
    indexingFilesPtr->push_back(fileStr);
  }

  auto* currentFilesPtr = access.accessValueWithAllocator<SharedMemory::Map<Id, SharedMemory::String>>(sCurrentFilesKeyName);
  if(currentFilesPtr != nullptr) {
    SharedMemory::Map<Id, SharedMemory::String>::iterator it = currentFilesPtr->find(getProcessId());
    if(it != currentFilesPtr->end()) {
      const size_t overestimationMultiplier = 3;
      const std::string crashedFilePath = it->second.c_str();

      size_t estimatedSize = 262144 + sizeof(SharedMemory::String) + crashedFilePath.size();
      estimatedSize *= overestimationMultiplier;

      while(access.getFreeMemorySize() < estimatedSize) {
        LOG_INFO(fmt::format("grow memory - est: {} size: {} free: {} alloc: {}",
                             estimatedSize,
                             access.getMemorySize(),
                             access.getFreeMemorySize(),
                             access.getMemorySize()));
        access.growMemory(access.getMemorySize());

        LOG_INFO("growing memory succeeded");

        currentFilesPtr = access.accessValueWithAllocator<SharedMemory::Map<Id, SharedMemory::String>>(sCurrentFilesKeyName);
        if(currentFilesPtr == nullptr) {
          return;
        }
      }

      auto* crashedFilesPtr = access.accessValueWithAllocator<SharedMemory::Vector<SharedMemory::String>>(sCrashedFilesKeyName);

      if(crashedFilesPtr) {
        crashedFilesPtr->push_back(it->second);
      }
    }

    SharedMemory::String str(access.getAllocator());
    str = utility::encodeToUtf8(filePath.wstr()).c_str();

    it = currentFilesPtr->insert(std::pair<Id, SharedMemory::String>(getProcessId(), str)).first;
    it->second = str;
  }
}

void InterprocessIndexingStatusManager::finishIndexingSourceFile() {
  SharedMemory::ScopedAccess access(&mSharedMemory);

  auto* currentFilesPtr = access.accessValueWithAllocator<SharedMemory::Map<Id, SharedMemory::String>>(sCurrentFilesKeyName);
  if(currentFilesPtr) {
    currentFilesPtr->erase(currentFilesPtr->find(getProcessId()), currentFilesPtr->end());
  }

  auto* finishedProcessIdsPtr = access.accessValueWithAllocator<SharedMemory::Queue<Id>>(sFinishedProcessIdsKeyName);
  if(finishedProcessIdsPtr) {
    finishedProcessIdsPtr->push_back(mProcessId);
  }
}

void InterprocessIndexingStatusManager::setIndexingInterrupted(bool interrupted) {
  SharedMemory::ScopedAccess access(&mSharedMemory);

  bool* indexingInterruptedPtr = access.accessValue<bool>(sIndexingInterruptedKeyName);
  if(indexingInterruptedPtr) {
    *indexingInterruptedPtr = interrupted;
  }
}

bool InterprocessIndexingStatusManager::getIndexingInterrupted() {
  SharedMemory::ScopedAccess access(&mSharedMemory);

  bool* indexingInterruptedPtr = access.accessValue<bool>(sIndexingInterruptedKeyName);
  if(indexingInterruptedPtr) {
    return *indexingInterruptedPtr;
  }

  return false;
}

Id InterprocessIndexingStatusManager::getNextFinishedProcessId() {
  SharedMemory::ScopedAccess access(&mSharedMemory);

  auto* finishedProcessIdsPtr = access.accessValueWithAllocator<SharedMemory::Queue<Id>>(sFinishedProcessIdsKeyName);
  if(finishedProcessIdsPtr && !finishedProcessIdsPtr->empty()) {
    Id processId = finishedProcessIdsPtr->front();
    finishedProcessIdsPtr->pop_front();
    return processId;
  }

  return 0;
}

std::vector<FilePath> InterprocessIndexingStatusManager::getCurrentlyIndexedSourceFilePaths() {
  SharedMemory::ScopedAccess access(&mSharedMemory);

  std::vector<FilePath> indexingFiles;

  auto* indexingFilesPtr = access.accessValueWithAllocator<SharedMemory::Queue<SharedMemory::String>>(sIndexingFilesKeyName);
  if(indexingFilesPtr) {
    while(!indexingFilesPtr->empty()) {
      indexingFiles.emplace_back(utility::decodeFromUtf8(indexingFilesPtr->front().c_str()));
      indexingFilesPtr->pop_front();
    }
  }

  return indexingFiles;
}

std::vector<FilePath> InterprocessIndexingStatusManager::getCrashedSourceFilePaths() {
  std::vector<FilePath> crashedFiles;

  SharedMemory::ScopedAccess access(&mSharedMemory);

  auto* crashedFilesPtr = access.accessValueWithAllocator<SharedMemory::Vector<SharedMemory::String>>(sCrashedFilesKeyName);

  if(crashedFilesPtr) {
    for(const auto& i : *crashedFilesPtr) {
      crashedFiles.emplace_back(utility::decodeFromUtf8(i.c_str()));
    }
  }

  auto* currentFilesPtr = access.accessValueWithAllocator<SharedMemory::Map<Id, SharedMemory::String>>(sCurrentFilesKeyName);
  if(currentFilesPtr) {
    for(SharedMemory::Map<Id, SharedMemory::String>::iterator it = currentFilesPtr->begin(); it != currentFilesPtr->end(); it++) {
      crashedFiles.emplace_back(utility::decodeFromUtf8(it->second.c_str()));
    }
  }

  return crashedFiles;
}
