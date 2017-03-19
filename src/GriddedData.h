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
#include <map>

#include "GribRecord.h"
#include "VectorUtilities.h"
#include "Matrix.h"
#include "Tensor.h"
#include "Variables.h"

#ifndef _GRIDDED_DATA_H_
#define _GRIDDED_DATA_H_

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
        variable_t get_variable() const { return m_variable; }
        void set_variable(const variable_t new_variable) {
          m_variable = new_variable;
        }

        // get/set valid time.
        time_t get_valid_time() const { return m_valid_time; }
        void set_valid_time(const time_t valid_time) {
          m_valid_time = valid_time;
        }

        // get/set reference time
        time_t get_reference_time() const { return m_valid_time; }
        void set_reference_time(const time_t valid_time) {
          m_valid_time = valid_time;
        }

        // get/set individual values
        void set(int i, int j, double v) { m_values.set(i, j, v); }
        double get(int i, int j) { return m_values.get(i, j); }

        // Number of East-West then North-South points.
        std::vector<int> shape() const { return m_values.shape(); }

        // Is a point within the extent of the grid?
        inline bool   contains_point(double lon, double lat) const;

        void   print();

    protected:
        Matrix<double> m_values;
        Matrix<double> m_lons;
        Matrix<double> m_lats;

        variable_t m_variable;

        time_t m_valid_time;      // the time the variable is valid for.
        time_t m_reference_time;      // the time the variable was produced
};


class ForecastVariable
{
   public:

       GriddedVariable get_time(time_t t);

   protected:

       time_t m_reference_time; // The time the forecast was produced
       variable_t m_variable;
       int m_realization;

       std::vector<time_t> m_times; // An array of valid times
       Matrix<double> m_lons;
       Matrix<double> m_lats;

};


class EnsembleForecastVariable
{
    protected:

        VariableID m_id;
        Tensor<double> m_data;
        std::vector<time_t> *m_times; // An array of valid times
        std::vector<int> *m_realizations;
        Matrix<double> *m_lons;
        Matrix<double> *m_lats;

    public:

        EnsembleForecastVariable(VariableID id,
                                 Tensor<double> &data,
                                 std::vector<time_t> *time,
                                 std::vector<int> *realization,
                                 Matrix<double> *lons,
                                 Matrix<double> *lats){
            m_id = id;
            m_data = data;
            m_times = time;
            m_realizations = realization;
            m_lons = lons;
            m_lats = lats;
        };

        Tensor<double> get_data() {
            return m_data;
        }

        Matrix<double>* get_lons() {
            return m_lons;
        }

        Matrix<double>* get_lats() {
            return m_lats;
        }

        ForecastVariable get_realization(int i);
};


class EnsembleForecast
{
    protected:

        std::map<VariableID, Tensor<double>> m_variables;
        std::vector<int> m_realizations;
        std::vector<time_t> m_times; // An array of valid times
        Matrix<double> m_lons;
        Matrix<double> m_lats;

    public:

        EnsembleForecast() {};

        EnsembleForecast(std::vector<time_t> time,
                         std::vector<int> realization,
                         std::vector<double> lons,
                         std::vector<double> lats);

        std::vector<int> shape();

        void add_variable(VariableID id, Tensor<double> var);

        EnsembleForecastVariable get_variable(VariableID id);

};

#endif

