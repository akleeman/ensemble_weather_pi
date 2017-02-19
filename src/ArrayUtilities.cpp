#include "ArrayUtilities.h"


double nanmin(double const *values){
    /*
     * Returns the minimum value in an array ignoring any NaN values.
     */
    double min = values[0];
    int size = sizeof(values) / sizeof(min);
    for ( int i=1;  i < size;  ++i )
        // by using fmin instead of direct comparison this function
        // can handle NaNs in values.
        min = fmin(min, values[i]);
    return min;
}
