# ${CMAKE_SOURCE_DIR}/src/console/CMakeLists.txt
project(SourcetrailConsole CXX)

add_executable(
        SourcetrailConsole
        main.cpp)

target_link_libraries(
        SourcetrailConsole
        PRIVATE
        Qt5::Core
        fmt::fmt
        Sourcetrail::lib)

set_target_properties(
    SourcetrailConsole
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app/")