#pragma once
#include <gmock/gmock.h>

#include "DialogView.h"

struct MockedDialogView final : DialogView {
  explicit MockedDialogView() : DialogView(UseCase::INDEXING, nullptr) {}
};