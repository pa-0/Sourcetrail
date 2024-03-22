# ${CMAKE_SOURCE_DIR}/src/messaging/CMakeLists.txt
add_library(Sourcetrail_messaging)
add_library(Sourcetrail::messaging ALIAS Sourcetrail_messaging)

target_sources(
  Sourcetrail_messaging
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/utility/messaging/MessageBase.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/messaging/MessageListenerBase.cpp
          ${CMAKE_SOURCE_DIR}/src/lib/utility/messaging/MessageQueue.cpp)

target_include_directories(Sourcetrail_messaging PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/utility/messaging/)

target_link_libraries(
  Sourcetrail_messaging
  PUBLIC Sourcetrail::lib # TODO(Hussein): Needed for `type.h` It should be removed to Sourcetrail::utility
         Sourcetrail::lib_utility
  PRIVATE Sourcetrail::core Sourcetrail::scheduling)

if(ENABLE_UNIT_TEST)
  add_subdirectory(tests)
endif()
