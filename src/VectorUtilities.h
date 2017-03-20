#include <iostream>
#include <cmath>
#include <vector>

/*
 * Returns the index into a sorted array of values which
 * indexes where in the array the `bisection` value should
 * be placed to preserve order.
 */
int bisect(std::vector<double> values, double bisection);

int argmin(std::vector<double> values);

std::vector<int> argsort(std::vector<double> values);

double nanmin(std::vector<double> values);

double nanmax(std::vector<double> values);
