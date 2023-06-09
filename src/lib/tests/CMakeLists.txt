# ${CMAKE_SOURCE_DIR}/src/lib/tests/CMakeLists.txt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

add_library(
  lib_test_utilities
  utilities/FileHandler.cpp
)

target_include_directories(
  lib_test_utilities
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

set(
  test_lib_names
  FileHandlerTestSuite
  CommandLineParserTestSuite
  CommandlineHelperTestSuite
  CommandlineCommandTestSuite
  #ConsoleControllerTestSuite
  #ConsoleInterpreterTestSuite
  LocationTypeTestSuite
  SourceLocationTestSuite
  SourceLocationFileTestSuite
  SourceLocationCollectionTestSuite
  FindCommandTestSuite
)

foreach(test_name IN LISTS test_lib_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
      lib_test_utilities
      GTest::gmock
      GTest::gtest
      GTest::gtest_main
      Sourcetrail::lib)

  gtest_discover_tests(
    ${test_name}
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/"
      TEST_PREFIX "unittests.")
endforeach()
