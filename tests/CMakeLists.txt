# ${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt
if(ENABLE_INTEGRATION_TEST)
  add_subdirectory(integration)
endif()

if(ENABLE_E2E_TEST)
  add_subdirectory(e2e)
endif()