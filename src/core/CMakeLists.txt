#${CMAKE_SOURCE_DIR}/src/src/core/CMakeLists.txt
add_library(Sourcetrail_core)
add_library(Sourcetrail::core ALIAS Sourcetrail_core)

target_sources(
  Sourcetrail_core
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FileInfo.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FilePath.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FileRegister.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FileTree.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FileManager.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FilePathFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/FileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/utilityFile.cpp

    ${CMAKE_SOURCE_DIR}/src/lib/utility/ConfigManager.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/ScopedFunctor.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/TimeStamp.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/tracing.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/utility.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/utilityUuid.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/utilityXml.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/Version.cpp

    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/ConsoleLogger.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/FileLogger.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/Logger.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/LogManager.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/LogManagerImplementation.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/LogMessage.cpp

    ${CMAKE_SOURCE_DIR}/src/lib/utility/text/TextAccess.cpp)

target_include_directories(
  Sourcetrail_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/lib/utility/
    ${CMAKE_SOURCE_DIR}/src/lib/utility/file/
    ${CMAKE_SOURCE_DIR}/src/lib/utility/logging/
    ${CMAKE_SOURCE_DIR}/src/lib/utility/text/)

target_link_libraries(
  Sourcetrail_core
  PUBLIC
    Boost::date_time)

target_link_libraries(
  Sourcetrail_core
  PRIVATE
    Boost::filesystem
    Sourcetrail::lib_utility
    TinyXML::TinyXML
    fmt::fmt)

# ================================================
add_library(Sourcetrail_scheduling)
add_library(Sourcetrail::scheduling ALIAS Sourcetrail_scheduling)

target_sources(
  Sourcetrail_scheduling
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/Blackboard.cpp
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

target_include_directories(
  Sourcetrail_scheduling
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/lib/utility/scheduling/)

target_link_libraries(
  Sourcetrail_scheduling
  PRIVATE
    Boost::date_time
    Sourcetrail::core
    Sourcetrail::lib_utility)