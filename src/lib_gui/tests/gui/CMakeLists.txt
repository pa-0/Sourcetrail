# ${CMAKE_SOURCE_DIR}/src/lib_gui/tests/CMakeLists.txt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY )

set(test_lib_gui_names QtScreenSearchBoxTestSuite)

foreach(test_name IN LISTS test_lib_gui_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
      lib::mocks
      Qt5::Test
      Sourcetrail::lib_gui)

  set_target_properties(
    ${test_name}
    PROPERTIES
      AUTOMOC ON
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

  add_test(${test_name} ${test_name})
endforeach()

