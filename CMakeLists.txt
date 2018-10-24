cmake_minimum_required(VERSION 2.8)
project(fastply LANGUAGES CXX)

# Compiler requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions ("-Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual")

# Fastply
include_directories(include)

# Build some examples
option(BUILD_EXAMPLES "Build fastply examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif(BUILD_EXAMPLES)

# Unit Tests
option(BUILD_UNIT_TESTS "Build fastply unit tests" ON)
if(BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(BUILD_UNIT_TESTS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})