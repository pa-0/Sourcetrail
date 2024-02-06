# ${CMAKE_SOURCE_DIR}/src/core/tests/CMakeLists.txt

set(gtest_lib_names
    ConfigManagerTestSuite
    FileManagerTestSuite
    FilePathFilterTestSuite
    FilePathTestSuite
    FileSystemTestSuite
    LoggerTestSuite
    TextAccessTestSuite
    VersionTestSuite
    ScopedFunctorTestSuite)

foreach(test_name IN LISTS gtest_lib_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC GTest::gmock
           GTest::gtest
           GTest::gtest_main
           lib::mocks
           Sourcetrail::core)

  set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

  gtest_discover_tests(
    ${test_name}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/"
    TEST_PREFIX "unittests.")
endforeach()

target_link_libraries(FileSystemTestSuite PRIVATE Sourcetrail::lib_utility)

target_link_libraries(FileManagerTestSuite PRIVATE Sourcetrail::lib_utility)
