# ${CMAKE_SOURCE_DIR}/src/lib/tests/CMakeLists.txt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

set(
  test_lib_names
  CommandLineParserTestSuite
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
      GTest::gmock
      GTest::gtest
      GTest::gtest_main
      Sourcetrail::lib)

  gtest_discover_tests(
    ${test_name}
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/"
      TEST_PREFIX "unittests.")
endforeach()
