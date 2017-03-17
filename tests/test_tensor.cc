#include "Tensor.h"
#include <gtest/gtest.h>


TEST(tensor, test_constructors) {
  // Test without a data argument
  Tensor<double> ten({3, 4});
  ten = Tensor<double>({3, 4, 5});

  // Test with data that already exists.
  std::vector<double> data(30);

  data[0] = 1.;
  data[1] = 2.;
  data[1 + 2 * 1] = 3.;
  data[1 + 2 * 1 + 3 * 2 * 1] = 4.;
  data[1 + 3 * 2 * 1] = 5.;
  ten = Tensor<double>({2, 3, 5}, &data);
  // Make sure the correct data is retrieved
  ASSERT_EQ(ten.get({0, 0, 0}), 1.);
  ASSERT_EQ(ten.get({1, 0, 0}), 2.);
  ASSERT_EQ(ten.get({1, 1, 0}), 3.);
  ASSERT_EQ(ten.get({1, 1, 1}), 4.);
  ASSERT_EQ(ten.get({1, 0, 1}), 5.);

  //Test with integer types
  Tensor<int> imat({4, 3});
}
