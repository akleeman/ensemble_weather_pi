#include <gtest/gtest.h>
#include "../src/GriddedData.h"

GriddedVariable makeTestData() {

  std::vector<double> lon_vals = {-121., -120.5, -120., -119.5, -119.};
  std::vector<double> lat_vals = {37., 37.5, 38., 38.5, 39., 39.5, 40.};

  Matrix<double> lons;
  Matrix<double> lats;

  meshgrid(lon_vals, lat_vals, &lons, &lats);

//  Matrix<double> data;
  GriddedVariable var(&lons, &lats);
  return var;
}

TEST(gridded_data, test_meshgrid){
  std::vector<double> lon_vals = {-121., -120.5, -120., -119.5, -119.};
  std::vector<double> lat_vals = {37., 37.5, 38., 38.5, 39., 39.5, 40.};

  Matrix<double> lons;
  Matrix<double> lats;

  meshgrid(lon_vals, lat_vals, &lons, &lats);

  EXPECT_DOUBLE_EQ(lons.get(2, 1), lon_vals[2]);
  EXPECT_DOUBLE_EQ(lons.get(2, 3), lon_vals[2]);
  EXPECT_DOUBLE_EQ(lats.get(2, 1), lat_vals[1]);
  EXPECT_DOUBLE_EQ(lats.get(4, 1), lat_vals[1]);
}


TEST(gridded_data, test_get_set) {
  GriddedVariable testVariable = makeTestData();
}
