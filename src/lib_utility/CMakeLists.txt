# ${CMAKE_SOURCE_DIR}/src/lib_utility/CMakeLists.txt
add_library(Sourcetrail_lib_utility)
add_library(Sourcetrail::lib_utility ALIAS Sourcetrail_lib_utility)

target_sources(
  Sourcetrail_lib_utility
  PRIVATE
    utility/TextCodec.cpp
    utility/utilityString.cpp)

target_include_directories(
  Sourcetrail_lib_utility
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/utility/) # TODO(Hussein): Should be removed

target_link_libraries(
  Sourcetrail_lib_utility
  PRIVATE
    project_options
    project_warnings
    Boost::system
    Qt5::Core)

if(UNIX AND NOT APPLE)
  find_package(Threads REQUIRED)
  target_link_libraries(
    Sourcetrail_lib_utility
    PRIVATE
      ${CMAKE_DL_LIBS}
      rt
      Threads::Threads)
endif()
