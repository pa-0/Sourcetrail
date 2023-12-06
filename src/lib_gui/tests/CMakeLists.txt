# ${CMAKE_SOURCE_DIR}/src/lib_gui/tests/CMakeLists.txt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

set(
  test_lib_gui_names
  QtNetworkFactoryTestSuite
  globalStringsTestSuite
  utilityAppTestSuite
)

foreach(test_name IN LISTS test_lib_gui_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
      GTest::gmock
      GTest::gtest
      GTest::gtest_main
      Sourcetrail::lib
      Sourcetrail::lib_gui
      $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:Sourcetrail::lib_cxx>)

  target_include_directories(
    ${test_name}
    PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}
      ${LIB_INCLUDE_PATHS}
      ${LIB_UTILITY_INCLUDE_PATHS}
      ${LIB_GUI_INCLUDE_PATHS}
      ${EXTERNAL_INCLUDE_PATHS}
      ${CMAKE_BINARY_DIR}/src/lib)

  gtest_discover_tests(
    ${test_name}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test/"
    TEST_PREFIX "unittests.")
endforeach()

# TODO(Hussein): Disable gui for now
#add_subdirectory(gui)
