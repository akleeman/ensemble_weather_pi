#include "VectorUtilities.h"
#include <gtest/gtest.h>
#include <iostream>

//
//#include <cmath>
//double nanmax(std::vector<double> values){
//    /*
//     * Returns the maximum value in an array ignoring any NaN values.
//     */
//    double max = values[0];
//    int n = values.size();
//    for ( int i=1;  i < n;  ++i )
//        // by using fmax instead of direct comparison this function
//        // can handle NaNs in values.
//        max = fmax(max, values[i]);
//    return max;
//}

TEST(vector_utils, test_tests) {
  std::vector<double> test_vector(5, 0.);

  test_vector[0] = 0.;
  test_vector[1] = 1.;
  test_vector[2] = 3.;
  test_vector[3] = 2.;
  test_vector[4] = -1.;

  EXPECT_TRUE(nanmax(test_vector) == 3.);
}
