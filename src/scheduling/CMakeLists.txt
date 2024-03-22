# ${CMAKE_SOURCE_DIR}/src/scheduling/CMakeLists.txt
add_library(Sourcetrail_scheduling)
add_library(Sourcetrail::scheduling ALIAS Sourcetrail_scheduling)

target_sources(
  Sourcetrail_scheduling
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/Blackboard.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/Task.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskDecorator.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskDecoratorRepeat.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskDecoratorDelay.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskFindKeyOnBlackboard.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskGroup.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskGroupParallel.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskGroupSelector.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskGroupSequence.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskLambda.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskManager.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskRunner.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/TaskScheduler.cpp)

target_include_directories(Sourcetrail_scheduling PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/)

target_link_libraries(
  Sourcetrail_scheduling
  PUBLIC Sourcetrail::core
  PRIVATE Boost::date_time Sourcetrail::core Sourcetrail::lib_utility)

if(ENABLE_UNIT_TEST)
  add_subdirectory(tests)
endif()
