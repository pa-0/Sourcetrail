#pragma once
// internal
#include "types.h"

struct StorageComponentAccess {
  StorageComponentAccess() = default;

  StorageComponentAccess(Id nodeId_, int type_) : nodeId(nodeId_), type(type_) {}

  bool operator<(const StorageComponentAccess& other) const {
    return nodeId < other.nodeId;
  }

  Id nodeId = 0;
  int type = 0;
};
