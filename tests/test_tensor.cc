#include "Tensor.h"
#include <gtest/gtest.h>


TEST(tensor, test_constructors) {
  // Test without a data argument
  Tensor<double> ten({3, 4});
  ten = Tensor<double>({3, 4, 5});

  // Test initializing with an array of data.
  std::vector<double> data;
  for (int i = 0; i < 30; i++){
    data.push_back((float) i);
  }
  ten = Tensor<double>({2, 3, 5}, &data);

  //Test with integer types
  Tensor<int> imat({4, 3});
}


TEST(tensor, test_get_set) {
    // Test initializing with an array of data.
    std::vector<double> data;
    for (int i = 0; i < 30; i++){
      data.push_back((float) i);
    }

    Tensor<double> ten({2, 3, 5}, &data);

    // The idea is to preserve the indexing order exhibited by
    // python (which compresses the data used by slocum).
    //
    // data = np.arange(30).reshape((2, 3, 5))
    //
    //  In [8]: data[1, 0, 0]
    //  Out[8]: 15
    //
    //  In [9]: data[1, 1, 0]
    //  Out[9]: 20
    //
    //  In [10]: data[1, 1, 1]
    //  Out[10]: 21
    //
    //  In [11]: data[1, 0, 1]
    //  Out[11]: 16

    // Make sure the correct data is retrieved
    ASSERT_EQ(ten.get({0, 0, 0}), 0.);
    ASSERT_EQ(ten.get({1, 0, 0}), 15.);
    ASSERT_EQ(ten.get({1, 1, 0}), 20.);
    ASSERT_EQ(ten.get({1, 1, 1}), 21.);
    ASSERT_EQ(ten.get({1, 0, 1}), 16.);

    ten.set({0, 1, 0}, 1.);
    ASSERT_EQ(ten.get({0, 1, 0}), 1.);
}
