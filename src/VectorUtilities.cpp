#include "VectorUtilities.h"


int bisect(std::vector<double> values, double bisection){
    int i;
    for (i = 0; i < (int) values.size(); i++){
        if (values[i] >= bisection)
            break;
    }
    return i;
}


int argmin(std::vector<double> values){
  double min_value = values[0];
  int min_idx = 0;
  for (uint i=1; i<values.size(); i++){
    if (values[i] < min_value){
      min_value = values[i];
      min_idx = i;
    }
  }
  return min_idx;
}


std::vector<int> argsort(std::vector<double> values){
  std::vector<int> order;

  std::vector<double> copy(values);

  double max_val = nanmax(values);

  while(order.size() < copy.size()){
    int next_min = argmin(copy);
    order.push_back(next_min);
    copy[next_min] = max_val + 1.;
  }

  return order;
}


double nanmin(std::vector<double> values){
    /*
     * Returns the minimum value in an array ignoring any NaN values.
     */
    double min = values[0];
    int n = values.size();
    for ( int i=1;  i < n;  ++i )
        // by using fmin instead of direct comparison this function
        // can handle NaNs in values.
        min = fmin(min, values[i]);
    return min;
}


double nanmax(std::vector<double> values){
    /*
     * Returns the maximum value in an array ignoring any NaN values.
     */
    double max = values[0];
    int n = values.size();
    for ( int i=1;  i < n;  ++i )
        // by using fmax instead of direct comparison this function
        // can handle NaNs in values.
        max = fmax(max, values[i]);
    return max;
}
