cmake_minimum_required(VERSION 2.8)
project(fastply LANGUAGES CXX)

# Compiler requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions ("-Wall -Wno-deprecated-declarations")


# google-test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Fastply
include_directories(include)

# impl/examples
# add_subdirectory(src)

# FastPly Tests
add_subdirectory(test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})