set(TEST_SRC
    FastPlyTest.cpp
)

file(GLOB TEST_DATA
  "${CMAKE_CURRENT_SOURCE_DIR}/test_data/*.ply"
)

add_executable(test.bin ${TEST_SRC})
target_link_libraries(test.bin PRIVATE ${GTEST_BOTH_LIBRARIES})
install(FILES ${TEST_DATA} DESTINATION .)
add_custom_command(TARGET test.bin POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
      ${TEST_DATA}
     "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Copying test input")
add_custom_command(TARGET test.bin POST_BUILD COMMAND test.bin)
