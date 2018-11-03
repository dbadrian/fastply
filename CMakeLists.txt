cmake_minimum_required(VERSION 3.8)
project(fastply_dev)
# entry point for IDEs/developers

# The FastPly Library
add_subdirectory(src)

# Build Examples
# option(BUILD_EXAMPLES "Build examples" ON)
# if(BUILD_EXAMPLES)
#     add_subdirectory(example)
# endif(BUILD_EXAMPLES)

# Build Unit Tests
option(BUILD_UNIT_TESTS "Build unit tests" ON)
if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif(BUILD_UNIT_TESTS)