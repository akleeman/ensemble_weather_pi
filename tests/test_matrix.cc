#include "Matrix.h"
#include <gtest/gtest.h>


TEST(matrix, test_constructors) {
  Matrix<double> mat;

  // Test without a data argument
  mat = Matrix<double>(3, 4);

  // Test with data that already exists.
  std::vector<double> data(12);
  data[0] = 1.;
  data[1] = 2.;
  data[3] = 3.;
  mat = Matrix<double>(3, 4, &data);
  // Make sure the correct data is retrieved
  ASSERT_EQ(mat.get(0, 0), 1.);
  ASSERT_EQ(mat.get(1, 0), 2.);
  ASSERT_EQ(mat.get(0, 1), 3.);

  //Test with integer types
  Matrix<int> imat(4, 3);
}


int multiply(int i, int j) {
  return i * j;
}


TEST(matrix, test_apply) {
  Matrix<int> mat(4, 3);
  mat.apply(multiply);
}
