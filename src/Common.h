#include "wx/wx.h"
#include <cmath>
#include <vector>
#include <string>

#ifndef _COMMON_H_
#define _COMMON_H_

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
  WAVE_DIRECTION_ID,
  WIND_SPEED_ID,
  WIND_DIRECTION_ID,
};


struct compressed_variable_t {
    VariableID id;
    int length;
    std::string payload;
};


struct variable_t {
    std::string name;
    std::string long_name;
    std::vector<VariableID> dims;
    std::string units;
};

// Directions are stored in PI / 8 increments which let's
// us hold them in a 4 bit integer.  Note that these are
// the bin's edges.
const std::vector<double> direction_bins = {-15. * M_PI / 16.,
  -13. * M_PI / 16.,
  -11. * M_PI / 16.,
  -9. * M_PI / 16.,
  -7. * M_PI / 16.,
  -5. * M_PI / 16.,
  -3. * M_PI / 16.,
  -M_PI / 16.,
   M_PI / 16.,
   3. * M_PI / 16.,
   5. * M_PI / 16.,
   7. * M_PI / 16.,
   9. * M_PI / 16.,
   11. * M_PI / 16.,
   13. * M_PI / 16.,
   15. * M_PI / 16.,
  };

// wind is rounded to the nearest Beaufort Force.
const std::vector<double> wind_bins = {0., 1., 3., 6., 10., 16., 21.,
                                       27., 33., 40., 47., 55., 63., 75.};

// Here we define the colors used for bins.
const wxColour wind_colors[13] = {
    wxColour(215, 215, 215), // light grey
    wxColour(161, 238, 255), // lightest blue
    wxColour(66, 177, 229),  // light blue
    wxColour(66, 119, 229),  // pastel blue
    wxColour(96, 253, 75),   // green
    wxColour(28, 234, 0),    // yellow-green
    wxColour(251, 239, 54),  // yellow
    wxColour(251, 193, 54),  // orange
    wxColour(255, 79, 2),    // red
    wxColour(213, 12, 2),    // darker-red
    wxColour(255, 0, 192),   // red-purple
    wxColour(179, 13, 138),  // dark purple
    wxColour(0, 0, 0)        // black
};

//wxString pretty_time_delta(int seconds) {
//
//}
//static wxString ReadableTimeDelta(int seconds)
//{
//    if(seconds < 60)
//        return wxString::Format(_T("%02ds"), seconds);
//
//    if(seconds < 3600)
//        return wxString::Format(_T("%02d.%02d"), seconds / 60, seconds % 60);
//
//    if(seconds < 86400)
//        return wxString::Format(_T("%02d:%02d"), seconds / 3600, (seconds / 60) % 60);
//
//    return wxString::Format(_T("%dd %02dh"), seconds / 86400, seconds / 3600);
//}

#endif
