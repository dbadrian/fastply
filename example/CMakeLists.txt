# find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(generateLargePly generate_ply.cpp)
target_link_libraries(generateLargePly)

add_executable(readLargePly read_large_ply.cpp)
target_link_libraries(readLargePly)