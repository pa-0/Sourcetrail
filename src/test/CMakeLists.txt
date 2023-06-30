# ${CMAKE_SOURCE_DIR}/src/test/CMakeLists.txt
add_library(catch_main OBJECT test_main.cpp)
target_link_libraries(catch_main PUBLIC catch2::catch2 Sourcetrail::core)
target_include_directories(
  catch_main
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/helper
    ${CMAKE_SOURCE_DIR}/src/lib_gui/utility/
    ${CMAKE_SOURCE_DIR}/src/lib_gui/utility/path_detector/
    ${LIB_INCLUDE_PATHS}
    ${LIB_GUI_INCLUDE_PATHS}
    ${CMAKE_BINARY_DIR}/src/lib)

# ========================================================
add_library(testHelper helper/TestFileRegister.cpp)
target_link_libraries(
  testHelper
    PUBLIC
    Sourcetrail::core
    Sourcetrail::scheduling)
target_include_directories(
  testHelper
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/helper
    ${LIB_INCLUDE_PATHS})

# ========================================================
set(
  test_lib_names
  CommandlineTestSuite
  GraphTestSuite
  HierarchyCacheTestSuite
  LogManagerTestSuite
  LowMemoryStringMapTestSuite
  MatrixBaseTestSuite
  MatrixDynamicBaseTestSuite
  NetworkProtocolHelperTestSuite
  ProjectSettingsTestSuite
  SearchIndexTestSuite
  SettingsMigratorTestSuite
  SettingsTestSuite
  SharedMemoryTestSuite
  UtilityTestSuite
  Vector2TestSuite)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/")

foreach(test_name IN LISTS test_lib_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
      testHelper
      catch2::catch2
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
# ========================================================
set(test_core_names "")

foreach(test_name IN LISTS test_core_names)
  add_executable(${test_name} ${test_name}.cpp)

  target_link_libraries(
    ${test_name}
    PUBLIC
      testHelper
      catch2::catch2
      Sourcetrail::core)

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
# ========================================================
if(${BUILD_CXX_LANGUAGE_PACKAGE})
  set(
    test_cxx_names
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
endif()
# ========================================================
execute_process(
  COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CMAKE_BINARY_DIR}/test"
)
execute_process(
  COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_SOURCE_DIR}/bin/test/data" "${CMAKE_BINARY_DIR}/test/data"
)
