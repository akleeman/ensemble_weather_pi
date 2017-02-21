if (CMAKE_CROSSCOMPILING)
  message(STATUS "Skipping unit tests, cross compiling")
else (CMAKE_CROSSCOMPILING)
  enable_testing()
  include_directories(SYSTEM ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
  include_directories("${PROJECT_SOURCE_DIR}/include")

  # Enable errors for warnings after gtest is built
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fexceptions -frtti")
  set("LOG_LEVEL=8")

  add_executable(unit_tests
    test_array_utils.cc)

  target_link_libraries(unit_tests gtest gtest_main pthread)

  add_custom_command(
    TARGET unit_tests POST_BUILD
    COMMENT "Running unit tests"
    COMMAND unit_tests
  )

endif (CMAKE_CROSSCOMPILING)
