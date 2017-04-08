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

#include "wx/wx.h"
#include <cmath>
#include <vector>
#include <string>

#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * This module contains constants and definitions that are required across
 * multiple other modules.
 */

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

#endif
