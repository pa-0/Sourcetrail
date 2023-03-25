if(UNIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app/")
else()
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/app/")
  endforeach()
endif()

add_executable(${APP_INDEXER_NAME} main.cpp)

set_target_properties(
  ${APP_INDEXER_NAME}
  PROPERTIES
    OUTPUT_NAME sourcetrail_indexer)

if(WIN32)
  # hide the console when running a release build.
  set_target_properties(${APP_INDEXER_NAME} PROPERTIES LINK_FLAGS_DEBUG                   "/SUBSYSTEM:CONSOLE /DEBUG:FASTLINK")
  set_target_properties(${APP_INDEXER_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG          "_CONSOLE")
  set_target_properties(${APP_INDEXER_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO          "/SUBSYSTEM:CONSOLE")
  set_target_properties(${APP_INDEXER_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
  set_target_properties(${APP_INDEXER_NAME} PROPERTIES LINK_FLAGS_RELEASE                 "/ENTRY:\"mainCRTStartup\" /SUBSYSTEM:WINDOWS /DEBUG")
  set_target_properties(${APP_INDEXER_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL              "/SUBSYSTEM:WINDOWS")

  # generate pdb for release build
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
endif()

target_link_libraries(
  ${APP_INDEXER_NAME}
  PRIVATE
    $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:Sourcetrail::lib_cxx>
		Qt5::Core
    Sourcetrail::core
    Sourcetrail::lib
    Sourcetrail::lib_gui)

if(APPLE)
  find_library(CORE_FOUNDATION CoreFoundation)
  target_link_libraries(${APP_INDEXER_NAME} PRIVATE ${CORE_FOUNDATION})
endif ()

set_property(
  TARGET ${APP_INDEXER_NAME}
  PROPERTY INCLUDE_DIRECTORIES
    "${APP_INCLUDE_PATHS}"
    "${LIB_INCLUDE_PATHS}"
    "${LIB_UTILITY_INCLUDE_PATHS}"
    "${LIB_GUI_INCLUDE_PATHS}"
    "${CMAKE_BINARY_DIR}/src/lib_gui"
    "${CMAKE_BINARY_DIR}/src/lib")
