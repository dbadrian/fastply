# find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests FastPlyTest.cpp)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})

gtest_discover_tests(tests)

file(GLOB TEST_DATA
  "${CMAKE_CURRENT_SOURCE_DIR}/test_data/*.ply"
)

add_custom_command(TARGET tests POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
      ${TEST_DATA}
     "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Copying test input")
