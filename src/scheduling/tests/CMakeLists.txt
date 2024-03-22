# ${CMAKE_SOURCE_DIR}/src/scheduling/tests/CMakeLists.txt
set(test_lib_names TaskSchedulerTestSuite)

foreach(test_name IN LISTS test_lib_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(${test_name} PUBLIC GTest::gtest GTest::gtest_main Sourcetrail::scheduling)

  set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

  gtest_discover_tests(
    ${test_name}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/"
    TEST_PREFIX "unittests.")
endforeach()
