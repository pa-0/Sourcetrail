#include "ScopedFunctor.h"

ScopedFunctor::ScopedFunctor() : m_onDestroy([]() {}) {}

ScopedFunctor::ScopedFunctor(std::function<void(void)> onDestroy) : m_onDestroy(std::move(onDestroy)) {}

ScopedFunctor::~ScopedFunctor() {
  m_onDestroy();
}
