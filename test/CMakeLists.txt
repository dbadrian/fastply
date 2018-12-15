cmake_minimum_required(VERSION 3.8)
# project(fastply_test)

find_package(fastply CONFIG REQUIRED)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# GTest needs threading support
find_package(Threads)

# Compile Flags etc.
SET(TEST_DEBUG_OPTIONS -Wall -Wno-deprecated-declarations -pedantic -Wshadow -Wpointer-arith -Wcast-qual)

add_executable(tests FastPlyTest.cpp)
target_link_libraries(tests PRIVATE fastply::fastply)
target_link_libraries(tests PRIVATE ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(tests PRIVATE "$<$<CONFIG:DEBUG>:${TEST_DEBUG_OPTIONS}>")
# target_compile_options(foo PRIVATE "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")


gtest_discover_tests(tests)

# TODO: Glob'ing is bad, will be removed
file(GLOB TEST_DATA
  "${CMAKE_CURRENT_SOURCE_DIR}/test_data/*.ply"
)

add_custom_command(TARGET tests POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
           ${TEST_DATA}
          "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Copying test input")
