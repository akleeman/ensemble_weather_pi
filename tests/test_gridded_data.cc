#include <gtest/gtest.h>
#include "../src/GriddedData.h"

GriddedVariable make_test_gridded_variable() {

  std::vector<double> lon_vals = {-121., -120.5, -120., -119.5, -119.};
  std::vector<double> lat_vals = {37., 37.5, 38., 38.5, 39., 39.5, 40.};

  Matrix<double> lons;
  Matrix<double> lats;

  meshgrid(lon_vals, lat_vals, &lons, &lats);

  GriddedVariable var(&lons, &lats);
  return var;
}


EnsembleForecast make_test_forecast() {
  std::vector<double> lons = {-121., -120.5, -120., -119.5, -119.};
  std::vector<double> lats = {37., 37.5, 38., 38.5, 39., 39.5, 40.};
  std::vector<int> realization = {1, 2, 3, 4, 5};

  time_t now;
  gmtime(&now);
  tm *timeinfo = localtime(&now);
  std::vector<time_t> times(4);

  for (uint i = 0; i < times.size(); i++){
    timeinfo->tm_mday += 1;
    times[i] = mktime(timeinfo);
  }

  EnsembleForecast fcst(times, realization, lons, lats);

  std::vector<int> shape = fcst.shape();
//  int size = 1;
//  for (uint i=0; i < shape.size(); i++){
//    size *= shape[i];
//  }

  Tensor<double> wind_speed(shape);
  fcst.add_variable(WIND_SPEED_ID, wind_speed);

  Tensor<double> wind_direction(shape);
  fcst.add_variable(WIND_DIRECTION_ID, wind_direction);

  return fcst;
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


TEST(gridded_data, test_data_access){
  EnsembleForecast fcst = make_test_forecast();

  EnsembleForecastVariable wind_speed = fcst.get_variable(WIND_SPEED_ID);

  auto lons = wind_speed.get_lons();
  EXPECT_DOUBLE_EQ(lons->get(2, 1), -120.);

}


TEST(gridded_data, test_get_set) {
  GriddedVariable testVariable = make_test_gridded_variable();
}
