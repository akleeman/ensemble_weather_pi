
# This defaults to not running tests, to run them the user must call:
#   cmake -Dtest=ON
option(test "Run unit tests." OFF)

if (test)
  add_subdirectory(./googletest)

  enable_testing()
  
  set(CMAKE_C_FLAGS "-Wall -Wextra -std=c++11")
  
  include_directories(SYSTEM ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
  include_directories("${PROJECT_SOURCE_DIR}/src")
  
  # Enable errors for warnings after gtest is built
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fexceptions -frtti")
  set("LOG_LEVEL=8")
  
  SET(UTILS_SRC
      ../src/VectorUtilities.cpp
      ../src/Matrix.cpp
      ../src/GriddedData.cpp)
  
  ADD_LIBRARY(ensemble_weather_pi_utils SHARED ${UTILS_SRC})
  
  add_executable(unit_tests
    test_matrix.cc
    test_vector_utils.cc
    test_gridded_data.cc
    )
  
  target_link_libraries(unit_tests ensemble_weather_pi_utils gtest gtest_main pthread)
  set_property(TARGET unit_tests PROPERTY CXX_STANDARD 11)
  
  add_custom_command(
    TARGET unit_tests
    COMMENT "Running unit tests"
    COMMAND unit_tests
  )
endif (test)