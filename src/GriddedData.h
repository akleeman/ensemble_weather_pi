/**********************************************************************
OpenCPN - ensemble_weather_pi

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/******************************************************************************
 * ForecastData
 *
 * Holds classes that help dealing with forecast data, in particular
 * with forecast data that may contain multiple realizations (or ensembles)
 * for each location and time.
 *
 *****************************************************************************/

#include <tuple>

#include "GribRecord.h"
#include "VectorUtilities.h"
#include "Matrix.h"

struct variable_t {
  char* name;
  char* units;
  char* level;
  zuint level_value;
  zuchar level_type;
  zuint source;
};


void meshgrid(const std::vector<double> lons, std::vector<double> lats,
              Matrix<double> *lon_mat, Matrix<double> *lat_mat);


class GriddedVariable
{
    public:

        GriddedVariable(const Matrix<double> *lon = nullptr,
                        const Matrix<double> *lat = nullptr,
                        const Matrix<double> *data = nullptr,
                        const time_t *val_time = nullptr,
                        const time_t *ref_time = nullptr,
                        const variable_t *variable = nullptr);

        virtual ~GriddedVariable() {};

        // get/set variable descriptor
        variable_t get_variable() const { return _variable; }
        void set_variable(const variable_t new_variable) {
          _variable = new_variable;
        }

        // get/set valid time.
        time_t get_valid_time() const { return _valid_time; }
        void set_valid_time(const time_t valid_time) {
          _valid_time = valid_time;
        }

        // get/set reference time
        time_t get_reference_time() const { return _valid_time; }
        void set_reference_time(const time_t valid_time) {
          _valid_time = valid_time;
        }

        // get/set individual values
        void set(int i, int j, double v) { _values.set(i, j, v); }
        double get(int i, int j) { return _values.get(i, j); }

        // Number of East-West then North-South points.
        std::tuple<int, int> shape() const { return _values.shape(); }

        // Is a point within the extent of the grid?
        inline bool   contains_point(double lon, double lat) const;

        void   print();

    protected:
        Matrix<double> _values;
        Matrix<double> _lons;
        Matrix<double> _lats;

        variable_t _variable;

        time_t _valid_time;      // the time the variable is valid for.
        time_t _reference_time;      // the time the variable was produced
};
