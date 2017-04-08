/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Ensemble Weather Plugin
 * Author:   Alex Kleeman
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Alex Kleeman                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *
 */
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include "zlib.h"
#include "GriddedData.h"

#define DEBUG true
#define PRINT_DEBUG(x) if(DEBUG) std::cout << x << std::endl

// Be sure to only execute the following once.
#ifndef _SLOCUM_H_
#define _SLOCUM_H_

/*
 * This module deals with the unpacking and interpretation of forecasts
 * created using slocum (see github/akleeman/slocum.git).
 */

/*
 * Takes a string holding zlib compressed data and returns the unpacked data.
 */
std::string zlib_decompress(const std::string& str);

/*
 * Takes a string holding 32 bit integers that have been zlib compressed
 * and returns the corresponding unpacked vector.
 */
std::vector<int> expand_small_ints(std::string compressed);

/*
 * Takes a string holding float values that have been rounded to
 * some number of significant digits, then compressed as if they
 * were integers.  Returns the unpacked vector.
 */
std::vector<double> expand_small_floats(std::string compressed,
                                        int least_significant_digit);

/*
 * Unpacks a string (array of 8bit chars) which holds compressed 2 or 4 bit
 * integers into a vector of length `size`.  This is one of the core
 * functions to slocum compression.  Technically it works with 2 and 8
 * bit integers as well ... but 8 bit integers exist as atomic types and
 * 1 bit integers are just bools.
 */
std::vector<int> unpack_ints(std::string packed_array, int bits, int size);

/*
 * "tiny" arrays are arrays in which the values have been binned,
 * then the bin number can often be stored as a 4 bit integer
 * which can be very efficiently compressed.  Unpacking these values
 * requires knowing the bins used, the original shape, and an
 * optional feature in which the bins can represent wrapped values,
 * such as compass directions.
 */
std::vector<double> expand_tiny_array(std::string compressed,
                                      std::vector<double> bins,
                                      std::vector<int> shape,
                                      bool wrap=false,
                                      double wrap_value=0.);

/*
 * A convenience function which calls expand_tiny_array with all
 * the appropriate arguments for 16 bin directions.
 */
std::vector<double> expand_tiny_direction(std::string compressed,
                                          std::vector<int> shape);

/*
 * Expands a time variable which has been stored as a time origin
 * point and offsets from that point.  The origin is stored as an integer
 * holding the number of days since 00/00/00 00:00:00, followed
 * by an integers holding the number of seconds into that day.
 * Subsequent integers are offsets from that point in hours.
 */
std::vector<time_t> expand_time(std::string compressed);


/*
 * Realizations are always just a range.  For example an ensemble
 * forecast with 21 different forecasts will have realizations
 * with values 1, 2, ... 21.  To store these we simply store the
 * end value of the range (21 in the example).
 */
std::vector<int> expand_realization(std::string compressed);


/*
 * Slocum forecasts consiste of multiple variables that have been
 * concatenated together.  This function takes a (possibly partial)
 * string of encoded variables and returns a struct holding the
 * length, payload etc ... of the next variable.
 */
compressed_variable_t extract_one_variable(std::string encoded_variables);


/*
 * Takes a string of encoded variables and returns a map which maps
 * from variable id to structs holding the compressed data for that
 * variable.
 */
std::map<VariableID, compressed_variable_t> split_by_variable(std::string encoded_variables);


/*
 * Wind variables are stored in two parts, wind speed and direction.  This
 * takes a string holding the speed and direction (each as distinct
 * compressed variables), unpacks them and adds them to `fcst` (which
 * should already contain the coordinates time, realization, longitude, latitude).
 */
void expand_wind(std::string compressed, EnsembleForecast *fcst);


/*
 * Reads in a compressed slocum forecast and returns the result as an
 * EnsembleForecast which will contain any supported variables held in
 * the file.
 */
EnsembleForecast read_slocum_forecast(std::string filename);


/*
 * A function used when post processing ensemble forecasts which will convert
 * from a vector of integers containing bin numbers for some forecasts
 * and will return a new vector of probabilities which represents the probability
 * a frequentist would assign to each bin.  ie, prob[5] = 0.2 would indicate
 * that 20% of the integers in `bins` were 5.
 */
std::vector<double> bins_to_probabilities(std::vector<int> bins, int max_bin);


/*
 * Takes a vector of values and bin edges and will return the corresponding
 * bin value for each element.
 */
Matrix<int> values_to_bins(Matrix<double> values, std::vector<double> bins);


/*
 * A function which uses values_to_bins and bins_to_probabilities to
 * create a matrix of bin probabilities for a matrix of floating point values.
 */
Matrix<double> binned_probabilities(const Matrix<double> values,
                                    const std::vector<double> bin_divs);

/*
 * Returns a synthetic test forecasts which is used in some tests and
 * can be used to visually confirm the forecasts are being properly plotted.
 */
EnsembleForecast diagnostic_test_foreast();

#endif
