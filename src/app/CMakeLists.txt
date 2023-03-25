#${CMAKE_SOURCE_DIR}/src/app/CMakeLists.txt
if(UNIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app/")
else()
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/app/")
  endforeach()
endif()

if(WIN32)
  file(WRITE ${CMAKE_BINARY_DIR}/Sourcetrail.rc
    "// Icon with lowest ID value placed first to ensure application icon\n"
    "// remains consistent on all systems.\n"
    "IDI_ICON1               ICON                    \"${CMAKE_BINARY_DIR}/Sourcetrail.ico\"\n"
  )

  add_executable(Sourcetrail main.cpp ${CMAKE_BINARY_DIR}/Sourcetrail.rc)

  # also show the console when running a release build.
  set_target_properties(Sourcetrail PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE /DEBUG:FASTLINK")
  set_target_properties(Sourcetrail PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
  set_target_properties(Sourcetrail PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
  set_target_properties(Sourcetrail PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
  set_target_properties(Sourcetrail PROPERTIES LINK_FLAGS_RELEASE "/ENTRY:\"mainCRTStartup\" /SUBSYSTEM:CONSOLE /DEBUG")
  set_target_properties(Sourcetrail PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")

  # generate pdb for release build
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")

  set_property(
    TARGET Sourcetrail
      PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
      "${CMAKE_SOURCE_DIR}/bin/app")

  string(REGEX REPLACE "/" "\\\\" BACKSLASHED_CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})
  string(REGEX REPLACE "/" "\\\\" BACKSLASHED_CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
else()
  add_executable(Sourcetrail main.cpp)
endif()

target_link_libraries(
  Sourcetrail
  PUBLIC
    Sourcetrail::core
    ${LIB_GUI_PROJECT_NAME}
    $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:Sourcetrail::lib_cxx>
    ${LIB_PROJECT_NAME}
    Qt5::Widgets
    Qt5::Network)

target_include_directories(
  Sourcetrail
  PRIVATE
    "${APP_INCLUDE_PATHS}"
    "${LIB_INCLUDE_PATHS}"
    "${LIB_GUI_INCLUDE_PATHS}"
    "${CMAKE_BINARY_DIR}/src/lib_gui"
    "${CMAKE_BINARY_DIR}/src/lib")

target_include_directories(
  Sourcetrail
  SYSTEM PRIVATE
    "${EXTERNAL_INCLUDE_PATHS}")

# add platform specific libraries
if(APPLE)
  find_library(CORE_FOUNDATION CoreFoundation)
  target_link_libraries(Sourcetrail ${CORE_FOUNDATION})
endif ()

add_dependencies(Sourcetrail ${APP_INDEXER_NAME})
