#include "GriddedData.h"


variable_t time_variable = {.name = "time",
                            .long_name = "Time (utc)",
                            .dims = {TIME_ID},
                            };

variable_t wind_speed_variable = {.name = "wind_speed",
                                  .long_name = "Surface Wind Speed",
                                  .dims = {TIME_ID, REALIZATION_ID, LONGITUDE_ID, LATITUDE_ID},
                                  .units = "knots",
                                  };

variable_t wind_direction_variable = {.name = "wind_direction",
                                      .long_name = "Surface Wind Direction",
                                      .dims = {TIME_ID, REALIZATION_ID, LONGITUDE_ID, LATITUDE_ID},
                                      .units = "radians",
                                      };

//std::map<VariableID, variable_t> variable_from_id;
//variable_from_id[WIND_SPEED_ID] = wind_speed_variable;
//variable_from_id[WIND_DIRECTION_ID] = wind_direction_variable;


// Creates matrices of gridded values by specifying the x and y
// coordinates along the edges.  For example, when working with
// gridded data on a map it is often the case that the grid is
// defined along uniformly spaced latitudes and longitudes.  This
// function would take an array of longitudes (x) and latitudes (y)
// and create matrices for which the longitudes have been stacked
// row-wise and the latitudes have been stacked column wise.
void meshgrid(const std::vector<double> x, std::vector<double> y,
               Matrix<double> *x_mat, Matrix<double> *y_mat){
    // determine the new number of rows an columns
    int m = static_cast<int>(x.size());
    int n = static_cast<int>(y.size());
    // resize the output matrices to reflect the new size
    x_mat->reshape(m, n);
    y_mat->reshape(m, n);
    // iterate through and set the corresponding values.
    for(int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        x_mat->set(i, j, x[i]);
        y_mat->set(i, j, y[j]);
      }
    }
}


// Creates a GriddedVariable using
GriddedVariable::GriddedVariable(const Matrix<double> *lons,
                                 const Matrix<double> *lats,
                                 const Matrix<double> *data,
                                 const time_t *valid_time,
                                 const time_t *ref_time,
                                 const variable_t *variable){
  if (!lons){
    return;
  }

  m_lons = *lons;
  m_lats = *lats;


  int m, n;
  m = m_lons.shape()[0];
  n = m_lons.shape()[1];

  // The grid is defined by longitudes and latitudes, here
  // we make sure those are the same shape.
  assert(m_lons.shape() == m_lats.shape());


  if (data != NULL){
    // data is an optional argument, if it's provided we use and
    // and make sure its size agrees with the dimensions m and n.
    m_values = *data;
    std::vector<int> expected_shape = {m, n};
    assert(m_values.shape() == expected_shape);
  } else {
    // otherwise we just create an empty vector and assume the user
    // is going to fill it in later.
    m_values = Matrix<double>(m, n);
  }
  // Make sure the data matches the coordinates
  assert(m_lons.shape() == m_values.shape());

  // Check if we supplied a non-null valid time
  if (valid_time == NULL){
    time_t cur_time;
    time(&cur_time);
    valid_time = &cur_time;
  }
  m_valid_time = *valid_time;

  // Check if we supplied a non-null reference time
  if (ref_time == NULL){
    time_t cur_time;
    time(&cur_time);
    ref_time = &cur_time;
  }
  m_reference_time = *ref_time;

  // Check if we supplied a non-null variable descriptor
  if (variable == NULL){
    variable_t empty_variable;
    variable = &empty_variable;
  }
  m_variable = *variable;
}



SpotVariable SpotForecast::get_variable(VariableID id){
    return SpotVariable(m_lon, m_lat, m_times, m_realizations, m_variables[id]);
}


EnsembleForecast::EnsembleForecast(std::vector<time_t> time,
                                   std::vector<int> realization,
                                   std::vector<double> lons,
                                   std::vector<double> lats){
    m_times = time;
    m_realizations = realization;
    meshgrid(lons, lats, &m_lons, &m_lats);
}


bool EnsembleForecast::is_empty(){
    return m_times.empty();
}


std::vector<int> EnsembleForecast::shape(){
  return {(int) m_times.size(),
          (int) m_realizations.size(),
          (int) m_lons.shape()[0],
          (int) m_lats.shape()[1]};
}


void EnsembleForecast::add_variable(VariableID id, Tensor<double> var){
    m_variables.insert({id, var});
}


SpotForecast EnsembleForecast::get_spot(int lon_ind, int lat_ind){
    std::map<VariableID, Matrix<double>> spot_variables;

    int n_times = m_times.size();
    int n_realizations = m_realizations.size();
    for (auto it=m_variables.begin(); it!=m_variables.end(); ++it){

        Matrix<double> data({n_times, n_realizations});
        for (int i=0; i<n_times; i++){
            for (int j=0; j<n_realizations; j++){
                data.set(i, j, it->second.get({i, j, lon_ind, lat_ind}));
            }
        }

      spot_variables[it->first] = data;
    }
    SpotForecast spot(m_lons.get(lon_ind, lat_ind),
                      m_lats.get(lon_ind, lat_ind),
                      &m_times, &m_realizations,
                      spot_variables);
    return spot;
}


EnsembleForecastVariable EnsembleForecast::get_variable(VariableID id){
    return EnsembleForecastVariable(id, &m_variables[id], &m_times, &m_realizations,
                                    &m_lons, &m_lats);
}
