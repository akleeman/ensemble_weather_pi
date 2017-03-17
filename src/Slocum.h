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


enum VariableID {
  TIME_ID,
  REALIZATION_ID,
  LATITUDE_ID,
  LONGITUDE_ID,
  WIND_ID,
  CURRENT_ID,
  TEMPERATURE_ID,
  PRESSURE_ID,
  WAVE_HEIGHT_ID,
  WAVE_DIRECTION_ID
};


struct compressed_variable_t {
    VariableID id;
    int length;
    std::string payload;
};


struct variable_definition_t {
    std::string name;
    std::string long_name;
    std::vector<VariableID> dims;
    std::string units;
};


std::vector<int> expand_small_ints(std::string compressed);

std::vector<double> expand_small_floats(std::string compressed);

std::vector<double> expand_tiny_array(std::string compressed,
                                      std::vector<double> bins,
                                      std::vector<int> shape,
                                      bool wrap=false,
                                      double wrap_value=0.);

std::vector<double> expand_tiny_direction(std::string compressed,
                                          std::vector<int> shape);

std::vector<int> unpack_ints(std::string packed_array, int bits, int size);

std::string zlib_decompress(const std::string& str);

compressed_variable_t extract_one_variable(std::string encoded_variables);

std::map<VariableID, compressed_variable_t> split_by_variable(std::string encoded_variables);

EnsembleForecast read_slocum_forecast(std::string filename);

std::vector<time_t> expand_time(std::string compressed);

std::vector<int> expand_realization(std::string compressed);

double expand_wind(std::string compressed, std::vector<int> shape);

#endif
