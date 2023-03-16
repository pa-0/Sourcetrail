#pragma once
// STL
#include <vector>

template <typename T>
struct Tree {
  Tree() {}
  Tree(T data_) : data(data_) {}
  T data;
  std::vector<Tree<T>> children;
};
