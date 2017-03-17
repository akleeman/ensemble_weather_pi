#include "GriddedData.h"


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
  std::tie(m, n) = m_lons.shape();

  // The grid is defined by longitudes and latitudes, here
  // we make sure those are the same shape.
  assert(m_lons.shape() == m_lats.shape());


  if (data != NULL){
    // data is an optional argument, if it's provided we use and
    // and make sure its size agrees with the dimensions m and n.
    m_values = *data;
    std::tuple<int, int> expected_shape = std::make_tuple(m, n);
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
