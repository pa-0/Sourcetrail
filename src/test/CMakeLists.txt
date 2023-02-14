# ${CMAKE_SOURCE_DIR}/src/test/CMakeLists.txt
include(Catch)

add_library(catch_main OBJECT test_main.cpp)
target_link_libraries(catch_main PUBLIC catch2::catch2)
target_include_directories(
  catch_main
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/helper
    ${LIB_INCLUDE_PATHS}
    ${LIB_GUI_INCLUDE_PATHS}
    ${CMAKE_BINARY_DIR}/src/lib)

add_library(testHelper helper/TestFileRegister.cpp)
target_include_directories(
  testHelper
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/helper
    ${LIB_INCLUDE_PATHS})

set(
  test_names
  CommandlineTestSuite
  ConfigManagerTestSuite
  CxxIncludeProcessingTestSuite
  CxxParserTestSuite
  CxxTypeNameTestSuite
  FileManagerTestSuite
  FilePathFilterTestSuite
  FilePathTestSuite
  FileSystemTestSuite
  GraphTestSuite
  HierarchyCacheTestSuite
  LogManagerTestSuite
  LowMemoryStringMapTestSuite
  MatrixBaseTestSuite
  MatrixDynamicBaseTestSuite
  MessageQueueTestSuite
  NetworkProtocolHelperTestSuite
  RefreshInfoGeneratorTestSuite
  SearchIndexTestSuite
  SettingsMigratorTestSuite
  SettingsTestSuite
  SharedMemoryTestSuite
  SourceGroupTestSuite
  SourceLocationCollectionTestSuite
  SqliteBookmarkStorageTestSuite
  SqliteIndexStorageTestSuite
  StorageTestSuite
  TaskSchedulerTestSuite
  TextAccessTestSuite
  UtilityStringTestSuite
  UtilityTestSuite
  Vector2TestSuite)

if(UNIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")
else()
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/test/")
  endforeach()
endif()

foreach(test_name IN LISTS test_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
    testHelper
    catch2::catch2
    ${LIB_GUI_PROJECT_NAME}
    $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:${LIB_CXX_PROJECT_NAME}>
    ${LIB_PROJECT_NAME}
    ${LIB_GUI_PROJECT_NAME}
    $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:${LIB_CXX_PROJECT_NAME}>)

  target_include_directories(
    ${test_name}
    PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}
      ${LIB_INCLUDE_PATHS}
      ${LIB_UTILITY_INCLUDE_PATHS}
      ${LIB_GUI_INCLUDE_PATHS}
      ${EXTERNAL_INCLUDE_PATHS}
      ${Boost_INCLUDE_DIRS}
      ${CMAKE_BINARY_DIR}/src/lib
      $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:${LIB_CXX_INCLUDE_PATHS}>)

  # automatically discover ${test_name} that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
  # to whatever you want, or use different for different binaries
  catch_discover_tests(
    ${test_name}
    TEST_PREFIX
    "unittests."
    REPORTER XML
    OUTPUT_DIR .
    OUTPUT_PREFIX "unittests."
    OUTPUT_SUFFIX .xml
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test")
endforeach()

execute_process(
  COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CMAKE_BINARY_DIR}/test"
)
execute_process(
  COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_SOURCE_DIR}/bin/test/data" "${CMAKE_BINARY_DIR}/test/data"
)
