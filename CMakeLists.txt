#${CMAKE_SOURCE_DIR}/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
include(cmake/version.cmake)
project(Sourcetrail
        DESCRIPTION "interactive source explorer"
        VERSION     ${VERSION_STRING}
        HOMEPAGE_URL "opensourcesourcetrail.github.io"
        LANGUAGES C CXX)

set(BUILD_CXX_LANGUAGE_PACKAGE          OFF CACHE BOOL "Add C and C++ support to the Sourcetrail indexer.")
set(BUILD_DOC                           OFF CACHE BOOL "Build documentation.")
set(ENABLE_TEST                         OFF CACHE BOOL "Build tests.")
set(ENABLE_CLANG_TIDY                   OFF CACHE BOOL "Run clang-tidy after building.")
set(ENABLE_CPPCHECK                     OFF CACHE BOOL "Run cppcheck after building.")
set(ENABLE_SANITIZER_ADDRESS            OFF CACHE BOOL "Inject address sanitizer.")
set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR OFF CACHE BOOL "Inject undefined behavior sanitizer.")
set(ENABLE_SANITIZER_THREAD             OFF CACHE BOOL "Inject thread sanitizer.")
set(ENABLE_SANITIZER_MEMORY             OFF CACHE BOOL "Inject memory sanitizer.")
set(ENABLE_COVERAGE                     OFF CACHE BOOL "Build coverage report run `ninja coverage`.")
set(ENABLE_BUILD_WITH_TIME_TRACE        OFF CACHE BOOL "Trace building time.")

include(cmake/add_files.cmake)
include(cmake/version_setup.cmake)
include(cmake/licenses.cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  add_compile_options(-fcolor-diagnostics)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
endif()

include(cmake/set_alternate_linker.cmake)

set(USE_ALTERNATE_LINKER "" CACHE STRING "Use alternate linker. Leave empty for system default; alternatives are 'gold', 'lld', 'bfd', 'mold'")
if(NOT "${USE_ALTERNATE_LINKER}" STREQUAL "")
  set_alternate_linker(${USE_ALTERNATE_LINKER})
endif()

# Add project_options v0.26.3
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(
  _project_options
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL "https://github.com/aminya/project_options/archive/refs/tags/v0.26.3.zip")
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# Variables --------------------------------------------------------------------

set(PROJECT_NAME Sourcetrail)

set(APP_PROJECT_NAME         "${PROJECT_NAME}")
set(APP_INDEXER_NAME         "${PROJECT_NAME}_indexer")
set(LIB_GUI_PROJECT_NAME     "${PROJECT_NAME}_lib_gui")
set(LIB_PROJECT_NAME         "${PROJECT_NAME}_lib")

if(WIN32)
  set(PLATFORM_INCLUDE "includesWindows.h")
elseif(APPLE)
  set(PLATFORM_INCLUDE "includesMac.h")
else()
  set(PLATFORM_INCLUDE "includesLinux.h")
endif ()

set(BASH "")
if(WIN32)
  execute_process(COMMAND CMD /c where bash OUTPUT_VARIABLE BASH)
  string(REGEX REPLACE "\n$" "" BASH "${BASH}")
  message("bash: '${BASH}'")
endif()

# Project ----------------------------------------------------------------------

include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

if(BUILD_DOC)
  include(cmake/doxygen.cmake)
endif()

if(ENABLE_COVERAGE)
  set(ENABLE_COVERAGE "ENABLE_COVERAGE")
  configure_file("${CMAKE_SOURCE_DIR}/cmake/coverage.sh.in"
                 "${CMAKE_BINARY_DIR}/coverage.sh")
  file(CHMOD "${CMAKE_BINARY_DIR}/coverage.sh"
       FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
  add_custom_target(Coverage
    COMMAND ${CMAKE_BINARY_DIR}/coverage.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

project_options(
  ENABLE_CONAN
  # CONAN_OPTIONS    # Extra options to pass to conan
  # MSVC_WARNINGS    # Override the defaults for the MSVC warnings
  # CLANG_WARNINGS   # Override the defaults for the CLANG warnings
  # GCC_WARNINGS     # Override the defaults for the GCC warnings
  ${ENABLE_CPPCHECK}
  ${ENABLE_CLANG_TIDY}
  ${ENABLE_COVERAGE}
  ENABLE_VS_ANALYSIS
  # ENABLE_INTERPROCEDURAL_OPTIMIZATION
  # ENABLE_NATIVE_OPTIMIZATION
  ${ENABLE_DOXYGEN}
  ${ENABLE_SANITIZER_ADDRESS}
  ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
  ${ENABLE_SANITIZER_THREAD}
  ${ENABLE_SANITIZER_MEMORY}
  # ENABLE_PCH
  # PCH_HEADERS
  # WARNINGS_AS_ERRORS
  # ENABLE_INCLUDE_WHAT_YOU_USE
  # ENABLE_BUILD_WITH_TIME_TRACE
  # ENABLE_UNITY
)

#RPATH
if(UNIX AND NOT APPLE)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/lib/:$$ORIGIN/lib/")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_STANDARD            17)
set(CMAKE_C_STANDARD              11)

# Settings ---------------------------------------------------------------------

find_package(SQLite3 CONFIG REQUIRED)
find_package(TinyXML CONFIG REQUIRED)
find_package(Tracy   CONFIG REQUIRED)
find_package(fmt     CONFIG REQUIRED)

# Boost ------------------------------------------------------------------------

set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS ON CACHE BOOL "Set to ON to force the use of the static libraries.")
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.67 COMPONENTS system program_options filesystem date_time REQUIRED)

# Qt ---------------------------------------------------------------------------

set(QT_MIN_VERSION     "5.12.0")
set(QT_MIN_VERSION_HEX 0x051200)
find_package(Qt5 ${QT_MIN_VERSION} COMPONENTS Widgets PrintSupport Network Svg REQUIRED)

if(WIN32)
  find_package(Qt5 ${QT_MIN_VERSION} COMPONENTS WinExtras REQUIRED)
endif()

if(Qt5Widgets_FOUND)
  message(STATUS "Found Qt ${Qt5Widgets_VERSION_STRING}")

  # FIX: Qt was built with -reduce-relocations
  if(Qt5_POSITION_INDEPENDENT_CODE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
  endif()

  # The following define makes your compiler emit warnings if you use
  # any feature of Qt which as been marked as deprecated (the exact warnings
  # depend on your compiler). Please consult the documentation of the
  # deprecated API in order to know how to port your code away from it.
  add_definitions(-DQT_DEPRECATED_WARNINGS)

  # You can also make your code fail to compile if you use deprecated APIs.
  # In order to do so, uncomment the following line.
  # You can also select to disable deprecated APIs only up to a certain version of Qt.
  #DEFINES += QT_DISABLE_DEPRECATED_BEFORE=${QT_MIN_VERSION_HEX}    # disables all the APIs deprecated at or before the specified Qt version
  add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=${QT_MIN_VERSION_HEX})
endif()

# Setup ---------------------------------------------------------------------------
if(WIN32)
  get_filename_component(QT_BINARY_DIR "${QT_MOC_EXECUTABLE}" PATH)
  set(QT_PLUGINS_DIR "${QT_BINARY_DIR}/../plugins")

  function(COPY_QT_BINARIES IS_DEBUG IS_APP)
    set(SUFFIX "")
    if (IS_DEBUG)
      set(SUFFIX "d")
    endif()

    set(CONFIGURATION "Release")
    if (IS_DEBUG)
      set(CONFIGURATION "Debug")
    endif()

    if(CMAKE_CL_64)
      set(BITS "64")
    else()
      set(BITS "32")
    endif()

    set(TARGET "test")
    if (IS_APP)
      set(TARGET "app")

      file(GLOB MY_PUBLIC_HEADERS
        "${CMAKE_SOURCE_DIR}/setup/dynamic_libraries/win${BITS}/app/${CONFIGURATION}/*"
      )
      file(COPY ${MY_PUBLIC_HEADERS} DESTINATION "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/")

    endif()

    configure_file("${QT_BINARY_DIR}/Qt5Core${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt5Core${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_BINARY_DIR}/Qt5Gui${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt5Gui${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_BINARY_DIR}/Qt5Network${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt5Network${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_BINARY_DIR}/Qt5Svg${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt5Svg${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_BINARY_DIR}/Qt5Widgets${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt5Widgets${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_BINARY_DIR}/Qt5WinExtras${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt5WinExtras${SUFFIX}.dll" COPYONLY)

    configure_file("${QT_PLUGINS_DIR}/platforms/qwindows${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/platforms/qwindows${SUFFIX}.dll" COPYONLY)

    configure_file("${QT_PLUGINS_DIR}/styles/qwindowsvistastyle${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/styles/qwindowsvistastyle${SUFFIX}.dll" COPYONLY)

    configure_file("${QT_PLUGINS_DIR}/imageformats/qgif${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qgif${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qicns${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qicns${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qico${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qico${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qjpeg${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qjpeg${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qsvg${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qsvg${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qtga${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qtga${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qtiff${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qtiff${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qwbmp${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qwbmp${SUFFIX}.dll" COPYONLY)
    configure_file("${QT_PLUGINS_DIR}/imageformats/qwebp${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qwebp${SUFFIX}.dll" COPYONLY)
  endfunction(COPY_QT_BINARIES)

  COPY_QT_BINARIES(True True)
  COPY_QT_BINARIES(True False)
  COPY_QT_BINARIES(False True)
  COPY_QT_BINARIES(False False)

  configure_file("${CMAKE_SOURCE_DIR}/setup/icon/windows/sourcetrail.ico" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/sourcetrail.ico" COPYONLY)
endif()

add_subdirectory(src/external)
add_subdirectory(src/lib)
add_subdirectory(src/lib_gui)

set_source_files_properties(${EXTERNAL_FILES} PROPERTIES COMPILE_FLAGS "-w")

# Lib Utility ------------------------------------------------------------------
add_subdirectory(src/lib_utility)
# core ------------------------------------------------------------------
add_subdirectory(src/core)
# Lib Cxx ----------------------------------------------------------------------
if(BUILD_CXX_LANGUAGE_PACKAGE)
  add_subdirectory(src/lib_cxx)
endif()
# Lib --------------------------------------------------------------------------
add_library(${LIB_PROJECT_NAME} ${LIB_FILES} ${EXTERNAL_FILES})
add_library(Sourcetrail::lib ALIAS ${LIB_PROJECT_NAME})

target_include_directories(
  ${LIB_PROJECT_NAME}
  PUBLIC
    ${LIB_INCLUDE_PATHS}
    ${LIB_GUI_INCLUDE_PATHS}
    ${CMAKE_SOURCE_DIR}/src/lib_gui/utility/
    ${CMAKE_SOURCE_DIR}/src/lib_gui/utility/path_detector/
    ${CMAKE_BINARY_DIR}/src/lib)

target_include_directories(
  ${LIB_PROJECT_NAME}
  SYSTEM PUBLIC
    ${EXTERNAL_INCLUDE_PATHS})

target_link_libraries(
  ${LIB_PROJECT_NAME}
  PUBLIC
    project_options
    project_warnings)

target_link_libraries(
  ${LIB_PROJECT_NAME}
  PRIVATE
    Boost::program_options
    Sourcetrail::core
    Sourcetrail::lib_utility
    Sourcetrail::scheduling
    ${LIB_GUI_PROJECT_NAME}
    Boost::system
    Boost::filesystem
    Boost::date_time
    fmt::fmt
    TinyXML::TinyXML
    SQLite::SQLite3
    Tracy::TracyClient
    $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:Sourcetrail::lib_cxx>
    $<$<PLATFORM_ID:Windows>:bcrypt>)

target_compile_definitions(${LIB_PROJECT_NAME} PRIVATE TRACY_ENABLE=1)

#configure language package defines
configure_file("${CMAKE_SOURCE_DIR}/cmake/language_packages.h.in"
               "${CMAKE_BINARY_DIR}/src/lib/language_packages.h")
# Lib Gui ----------------------------------------------------------------------
add_library(Sourcetrail_lib_gui_utility)
add_library(Sourcetrail::lib_gui::utility ALIAS Sourcetrail_lib_gui_utility)

target_sources(
  Sourcetrail_lib_gui_utility
  PRIVATE
    src/lib_gui/utility/utilityApp.cpp
    src/lib_gui/utility/utilityPathDetection.cpp
    src/lib_gui/utility/path_detector/CombinedPathDetector.cpp
    src/lib_gui/utility/path_detector/PathDetector.cpp)

target_include_directories(
  Sourcetrail_lib_gui_utility
  PUBLIC
    src/lib_gui/utility/
    src/lib_gui/utility/path_detector/
    src/lib_gui/utility/path_detector/cxx_header/)

target_link_libraries(
  Sourcetrail_lib_gui_utility
  PRIVATE
    project_options
    project_warnings
    Boost::system
    Qt5::Core
    Sourcetrail::core
    Sourcetrail::lib_utility
    ${LIB_PROJECT_NAME})

# target for running versionnumber script
# workaround for running customcommand (ninja dependency cycle)
add_custom_target(versionnumber ALL)

add_library(
  ${LIB_GUI_PROJECT_NAME}
  ${LIB_GUI_FILES}
  ${CMAKE_BINARY_DIR}/src/lib_gui/productVersion.h)

add_library(Sourcetrail::lib_gui ALIAS ${LIB_GUI_PROJECT_NAME})

target_link_libraries(
  ${LIB_GUI_PROJECT_NAME}
  PUBLIC
    Sourcetrail::lib_utility
    ${LIB_PROJECT_NAME}
    Qt5::Widgets
    Qt5::Network
    Qt5::Svg
    $<$<PLATFORM_ID:Windows>:Qt5::WinExtras>)

target_link_libraries(
  ${LIB_GUI_PROJECT_NAME}
  PRIVATE
    fmt::fmt
    Sourcetrail::core
    Sourcetrail::scheduling
    $<$<BOOL:${BUILD_CXX_LANGUAGE_PACKAGE}>:Sourcetrail::lib_cxx>
    Sourcetrail::lib_gui::utility)

# command for versioning script
add_custom_command(
  TARGET versionnumber
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -DBINARY_DIR=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/cmake/version.cmake
  BYPRODUCTS ${CMAKE_BINARY_DIR}/src/lib_gui/productVersion.h
  DEPENDS ${LIB_GUI_PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "check/update version number")

add_dependencies(${LIB_GUI_PROJECT_NAME} versionnumber)

# include external header without warnings
target_include_directories(
  ${LIB_GUI_PROJECT_NAME}
  PUBLIC
    ${LIB_GUI_INCLUDE_PATHS}
    ${LIB_INCLUDE_PATHS}
    ${CMAKE_BINARY_DIR}/src/lib_gui
    ${CMAKE_BINARY_DIR}/src/lib)

# include external header without warnings
target_include_directories(
  ${LIB_GUI_PROJECT_NAME}
  SYSTEM PUBLIC
    ${EXTERNAL_INCLUDE_PATHS})

# configure platform specific include file
configure_file("${PROJECT_SOURCE_DIR}/src/lib_gui/platform_includes/includes.h.in"
               "${PROJECT_BINARY_DIR}/src/lib_gui/includes.h")

#configure the versioning file
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.txt.in
               ${CMAKE_BINARY_DIR}/version.txt)

configure_file(${CMAKE_SOURCE_DIR}/cmake/productVersion.h.in
               ${CMAKE_BINARY_DIR}/src/lib_gui/productVersion.h)

set_target_properties(
  ${LIB_GUI_PROJECT_NAME}
  PROPERTIES
  AUTOMOC ON
  INCLUDE_CURRENT_DIR ON)

set_property(SOURCE ${CMAKE_BINARY_DIR}/src/lib_gui/productVersion.h
             PROPERTY SKIP_AUTOMOC ON)

# Indexer App ------------------------------------------------------------------
add_subdirectory(src/indexer)
# App --------------------------------------------------------------------------
add_subdirectory(src/app)
# macOS Bundle ----------------------------------------------------------------
if(APPLE)
  set(MACOS_BUNDLE_NAME ${PROJECT_NAME})
  set(MACOS_BUNDLE_VERSION ${VERSION_STRING})
  set(MACOS_BINARY_NAME ${APP_PROJECT_NAME})
  set(MACOS_INDEXER_BINARY_NAME ${APP_INDEXER_NAME})

  get_property(QT_CORE_PATH TARGET ${Qt5Core_LIBRARIES} PROPERTY LOCATION)
  get_filename_component(QT_CORE_PATH ${QT_CORE_PATH} REALPATH)

  get_property(QT_GUI_PATH TARGET ${Qt5Gui_LIBRARIES} PROPERTY LOCATION)
  get_filename_component(QT_GUI_PATH ${QT_GUI_PATH} REALPATH)

  get_property(QT_WIDGETS_PATH TARGET ${Qt5Widgets_LIBRARIES} PROPERTY LOCATION)
  get_filename_component(QT_WIDGETS_PATH ${QT_WIDGETS_PATH} REALPATH)

  get_property(QT_PRINT_PATH TARGET ${Qt5PrintSupport_LIBRARIES} PROPERTY LOCATION)
  get_filename_component(QT_PRINT_PATH ${QT_PRINT_PATH} REALPATH)

  get_property(QT_NETWORK_PATH TARGET ${Qt5Network_LIBRARIES} PROPERTY LOCATION)
  get_filename_component(QT_NETWORK_PATH ${QT_NETWORK_PATH} REALPATH)

  get_property(QT_SVG_PATH TARGET ${Qt5Svg_LIBRARIES} PROPERTY LOCATION)
  get_filename_component(QT_SVG_PATH ${QT_SVG_PATH} REALPATH)

  list(APPEND MACOS_QT_FRAMEWORKS ${QT_CORE_PATH} ${QT_GUI_PATH} ${QT_WIDGETS_PATH} ${QT_PRINT_PATH} ${QT_NETWORK_PATH} ${QT_SVG_PATH})
  string(REPLACE ";" " " MACOS_QT_FRAMEWORKS "${MACOS_QT_FRAMEWORKS}")

  set(MACOS_BOOST_DIR "${Boost_INCLUDE_DIR}")
  STRING(REGEX REPLACE "/lib/cmake/clang" "" MACOS_CLANG_DIR "${Clang_DIR}")
  STRING(REGEX REPLACE "/lib/cmake/Qt5" "" MACOS_QT_DIR "${Qt5_DIR}")

  configure_file(
    ${PROJECT_SOURCE_DIR}/setup/macOS/bundle_install.sh.in
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bundle_install.sh
    @ONLY
  )

  configure_file(
    ${PROJECT_SOURCE_DIR}/setup/macOS/bundle_info.plist.in
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bundle_info.plist
    @ONLY
  )
endif ()
# Test ----------------------------------------------------------------------
if(ENABLE_TEST)
  find_package(Catch2 CONFIG REQUIRED)

  enable_testing()
  add_subdirectory(src/test)
endif()

execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CMAKE_BINARY_DIR}/app")
execute_process(
  COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_SOURCE_DIR}/bin/app/data" "${CMAKE_BINARY_DIR}/app/data"
  COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_SOURCE_DIR}/bin/app/user" "${CMAKE_BINARY_DIR}/app/user")

if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

# Add other targets that you want installed here, be default we just package the one executable
# we know we want to ship
package_project(TARGETS ${APP_PROJECT_NAME} ${APP_INDEXER_NAME}
  # FIXME: this does not work! CK
  # PRIVATE_DEPENDENCIES_CONFIGURED project_options project_warnings
)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
