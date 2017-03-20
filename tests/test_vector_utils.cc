#include "VectorUtilities.h"
#include <gtest/gtest.h>


TEST(vector_utils, test_bisect) {

  std::vector<double> test_vector = {0., 1., 2., 3.};

  EXPECT_EQ(0, bisect(test_vector, 0.));
  EXPECT_EQ(0, bisect(test_vector, -1.));
  EXPECT_EQ(1, bisect(test_vector, 0.99));
  EXPECT_EQ(1, bisect(test_vector, 1.));
  EXPECT_EQ(3, bisect(test_vector, 2.01));
  EXPECT_EQ(4, bisect(test_vector, 4.));
}


TEST(vector_utils, test_argsort) {

  std::vector<double> test_vector = {1., 4., 3., 5.};

  std::vector<int> order = argsort(test_vector);

  std::vector<int> expected {0, 2, 1, 3};
  EXPECT_EQ(order, expected);

}

TEST(vector_utils, test_nanmax) {
  std::vector<double> test_vector;

  // Test with a simple vector
  test_vector = {0., 1., 3., -1., 2.};
  EXPECT_TRUE(nanmax(test_vector) == 3.);

  // Again but with repeated maxs
  test_vector = {3., 1., 3., -1., 2.};
  EXPECT_TRUE(nanmax(test_vector) == 3.);

  // Again but with a NaN

  test_vector = {3., 1., NAN, -1., 2.};
  EXPECT_TRUE(nanmax(test_vector) == 3.);
}


TEST(vector_utils, test_nanmin) {
  std::vector<double> test_vector;

  // Test with a simple vector
  test_vector = {0., 1., 3., -1., 2.};
  EXPECT_TRUE(nanmin(test_vector) == -1.);

  // Again but with repeated maxs
  test_vector = {3., 1., 3., -1., 2.};
  EXPECT_TRUE(nanmin(test_vector) == -1.);

  // Again but with a NaN

  test_vector = {3., 1., NAN, -1., 2.};
  EXPECT_TRUE(nanmin(test_vector) == -1.);
}
