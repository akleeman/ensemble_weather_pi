#include "VectorUtilities.h"


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
