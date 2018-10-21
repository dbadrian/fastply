# find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests FastPlyTest.cpp)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})

gtest_discover_tests(tests)
# add_test(NAME monolithic COMMAND tests)

file(GLOB TEST_DATA
  "${CMAKE_CURRENT_SOURCE_DIR}/test_data/*.ply"
)

add_custom_command(TARGET tests POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
      ${TEST_DATA}
     "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Copying test input")
# add_custom_command(TARGET tests POST_BUILD COMMAND tests)




# # google-test
# enable_testing()
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

# set(TEST_SRC
#     FastPlyTest.cpp
# )


# file(GLOB TEST_DATA
#   "${CMAKE_CURRENT_SOURCE_DIR}/test_data/*.ply"
# )

# add_executable(test ${TEST_SRC})
# target_link_libraries(test PRIVATE ${GTEST_BOTH_LIBRARIES})

# # gtest_add_tests(TARGET test)
# gtest_discover_tests(test)



# target_link_libraries(test.bin PRIVATE ${GTEST_BOTH_LIBRARIES})
# install(FILES ${TEST_DATA} DESTINATION .)
# add_custom_command(TARGET test.bin POST_BUILD
#   COMMAND "${CMAKE_COMMAND}" -E copy
#       ${TEST_DATA}
#      "${CMAKE_CURRENT_BINARY_DIR}"
#   COMMENT "Copying test input")
# add_custom_command(TARGET test.bin POST_BUILD COMMAND test.bin)
