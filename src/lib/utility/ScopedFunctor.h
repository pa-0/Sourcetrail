#pragma once
// STL
#include <functional>

class ScopedFunctor {
public:
  ScopedFunctor();
  ScopedFunctor(std::function<void(void)> onDestroy);
  ~ScopedFunctor();

private:
  std::function<void(void)> m_onDestroy;
};