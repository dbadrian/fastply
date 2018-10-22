cmake_minimum_required(VERSION 2.8)
project(fastply LANGUAGES CXX)

# Compiler requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions ("-Wall -Wno-deprecated-declarations -pedantic -Wshadow -Wpointer-arith -Wcast-qual -std=c++17")

include(third_party/GTestSrc.cmake)

# GTest needs threading support
find_package (Threads)

# Fastply
include_directories(include)

# impl/examples
# add_subdirectory(src)

# Build some examples
add_subdirectory(example)

# FastPly Tests
enable_testing()
add_subdirectory(test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})