# ${CMAKE_SOURCE_DIR}/src/lib_cxx/tests/CMakeLists.txt
set(test_cxx_names
    CxxIncludeProcessingTestSuite
    CxxParserTestSuite
    CxxTypeNameTestSuite)

foreach(test_name IN LISTS test_cxx_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
    testHelper
    catch2::catch2
    Sourcetrail::lib
    Sourcetrail::lib_cxx
    Sourcetrail::lib_utility)

  target_include_directories(
    ${test_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_PATHS}
    ${LIB_UTILITY_INCLUDE_PATHS}
    ${LIB_GUI_INCLUDE_PATHS}
    ${EXTERNAL_INCLUDE_PATHS}
    ${CMAKE_BINARY_DIR}/src/lib
    ${LIB_CXX_INCLUDE_PATHS})

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