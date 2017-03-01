#include "VectorUtilities.h"
#include <gtest/gtest.h>

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
