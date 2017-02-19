if (CMAKE_CROSSCOMPILING)
  message(STATUS "Skipping unit tests, cross compiling")
else (CMAKE_CROSSCOMPILING)
  enable_testing()
  include_directories(SYSTEM ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
  include_directories("${PROJECT_SOURCE_DIR}/include")

  # Enable errors for warnings after gtest is built
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fexceptions -frtti")
  set("LOG_LEVEL=8")
  set(CMAKE_CXX_FLAGS "-Wshadow -Wswitch-default -Wswitch-enum -Wundef -Wuninitialized -Wpointer-arith -Wcast-align -Wformat=2 -Wredundant-decls -Wformat-security ${CMAKE_CXX_FLAGS}")
  if (APPLE)
    # Set default stack usage to 32MB
    # See: https://github.com/swift-nav/estimation_team_planning/issues/216
    set(CMAKE_EXE_LINKER_FLAGS
      "-Wl,-stack_size -Wl,0x2000000 ${CMAKE_EXE_LINKER_FLAGS}")
  endif (APPLE)

  add_executable(slocum_pi_tests
    test_array_utils.cc)

  target_link_libraries(slocum_pi_tests gtest gtest_main pthread)

  add_custom_command(
    TARGET slocum_pi_tests POST_BUILD
    COMMENT "Running slocum_pi unit tests"
    COMMAND slocum_pi_tests
  )
endif (CMAKE_CROSSCOMPILING)
