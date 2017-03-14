#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "zlib.h"

#include "GriddedData.h"

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


class AbstractVariable {

  public:

    void decompress(compressed_variable_t compressed);

};


class TimeVariable : AbstractVariable {

  public:

    void decompress(compressed_variable_t compressed);

  private:

    std::vector<time_t> m_data;
};


std::string zlib_decompress(const std::string& str);

compressed_variable_t extract_one_variable(std::string encoded_variables);

GriddedVariable read_slocum_forecast(std::string filename);

