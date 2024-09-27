# ${CMAKE_SOURCE_DIR}/src/external/CMakeLists.txt
add_library(CppSQLite3 STATIC)
target_sources(CppSQLite3 PRIVATE sqlite/CppSQLite3.cpp)
add_library(CppSQLite::CppSQLite3 ALIAS CppSQLite3)
target_link_libraries(CppSQLite3 PUBLIC SQLite::SQLite3)
target_include_directories(CppSQLite3 SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/sqlite)
target_compile_options(CppSQLite3 PRIVATE $<$<PLATFORM_ID:Linux>:-w> $<$<PLATFORM_ID:Windows>:/w>)
