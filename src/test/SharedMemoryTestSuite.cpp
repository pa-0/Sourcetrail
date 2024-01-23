#include <memory>
#include <thread>

#include <gtest/gtest.h>

#include "SharedMemory.h"

TEST(SharedMemory, goodCase) {
  SharedMemory memory("memory", 1000, SharedMemory::CREATE_AND_DELETE);

  {
    SharedMemory::ScopedAccess access(&memory);

    EXPECT_TRUE(access.getMemorySize() == 1000);
    *access.accessValue<int>("count") = 0;
  }

  std::vector<std::shared_ptr<std::thread>> threads;
  for(unsigned int i = 0; i < 4; i++) {
    threads.push_back(std::make_shared<std::thread>([]() {
      SharedMemory memory("memory", 0, SharedMemory::OPEN_ONLY);

      SharedMemory::ScopedAccess access(&memory);

      if(access.getMemorySize() < 5000) {
        access.growMemory(5000 - access.getMemorySize());
      }

      int* count = access.accessValue<int>("count");
      *count += 1;

      SharedMemory::String* str = access.accessValueWithAllocator<SharedMemory::String>("string");
      str->append("hi");

      SharedMemory::Vector<int>* nums = access.accessValueWithAllocator<SharedMemory::Vector<int>>("nums");
      nums->push_back(static_cast<int>(nums->size()));

      SharedMemory::Vector<SharedMemory::String>* strings =
          access.accessValueWithAllocator<SharedMemory::Vector<SharedMemory::String>>("strings");
      strings->push_back(SharedMemory::String("ho", access.getAllocator()));

      SharedMemory::Map<int, int>* vals = access.accessValueWithAllocator<SharedMemory::Map<int, int>>("vals");
      vals->emplace(static_cast<int>(vals->size()), static_cast<int>(vals->size() * vals->size()));
    }));
  }

  for(auto& thread : threads) {
    thread->join();
  }
  threads.clear();

  {
    SharedMemory::ScopedAccess access(&memory);

    EXPECT_TRUE(access.getMemorySize() == 5000);
    EXPECT_TRUE(*access.accessValue<int>("count") == 4);
    const std::string value = access.accessValueWithAllocator<SharedMemory::String>("string")->c_str();
    EXPECT_TRUE(value == "hihihihi");

    auto* nums = access.accessValueWithAllocator<SharedMemory::Vector<int>>("nums");
    EXPECT_TRUE(nums->size() == 4);
    for(int i : *nums) {
      EXPECT_TRUE(i == i);
    }

    auto* strings = access.accessValueWithAllocator<SharedMemory::Vector<SharedMemory::String>>("strings");
    EXPECT_TRUE(strings->size() == 4);
    for(SharedMemory::String& str : *strings) {
      EXPECT_TRUE(str == "ho");
    }

    auto* vals = access.accessValueWithAllocator<SharedMemory::Map<int, int>>("vals");
    EXPECT_TRUE(vals->size() == 4);

    size_t i = 0;
    for(auto val : *vals) {
      EXPECT_TRUE(val.first == i);
      EXPECT_TRUE(val.second == i * i);
      i++;
    }
  }
}
