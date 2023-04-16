# ${CMAKE_SOURCE_DIR}/src/core/tests/CMakeLists.txt
set(
  test_lib_names
  FilePathTestSuite
  FilePathFilterTestSuite)

foreach(test_name IN LISTS test_lib_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
      catch2::catch2
      Sourcetrail::core)

  set_target_properties(
    ${test_name}
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

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