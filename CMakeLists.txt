cmake_minimum_required(VERSION 3.8)
# entry point for clion
# if you plan to use this library as
project(fastply_dev)

# The FastPly Library
add_subdirectory(src)

# Build Examples
# option(BUILD_EXAMPLES "Build fastply examples" ON)
# if(BUILD_EXAMPLES)
#     add_subdirectory(example)
# endif(BUILD_EXAMPLES)

# Build Unit Tests
option(BUILD_UNIT_TESTS "Build fastply unit tests" ON)
if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif(BUILD_UNIT_TESTS)