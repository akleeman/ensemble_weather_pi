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

#include "GribRecord.h"
#include "ArrayUtilities.h"

enum ForecastProvider {
    NOAA_GFS,
    NOAA_GEFS,
    NOAA_NCEP_WW3,
    NOAA_RTOFS,
};

//----------------------------------------------
class GriddedVariable
{
    public:
        GriddedVariable(const GriddedVariable &rec);
        GriddedVariable() {m_bfilled = false;}
        virtual ~GriddedVariable();

        const char* getVariableName() const { return variableName; }
        void setVariableName(const char* name);

        zuchar  getLevelType() const   { return levelType; }
        zuint   getLevelValue() const  { return levelValue; }
        zuint   getSource() const { return source; }

        // Number of points in the grid
        int getNi() const     { return Ni; }
        int getNj() const     { return Nj; }

        int getLat(const int i, const int j) const { return lats[j*Ni+i]; }
        int getLon(const int i, const int j) const { return lons[j*Ni+i]; }

        // Value at one point of the grid
        double getValue(const int i, const int j) const  { return data[j*Ni+i]; }

        void setValue(const int i, const int j, double v)
                        { if (i<Ni && j<Nj)
                              data[j*Ni+i] = v; }

        double  getLatMin() const   { return nanmin(&lats); }
        double  getLonMin() const   { return lonMin;}
        double  getLatMax() const   { return latMax;}
        double  getLonMax() const   { return lonMax;}

        // Is a point within the extent of the grid?
        inline bool   isPointInMap(double x, double y) const;
        inline bool   isXInMap(double x) const;
        inline bool   isYInMap(double y) const;
        // Is there a value at a particular grid point ?
        inline bool   hasValue(int i, int j) const;
        // Is there a value that is not GRIB_NOTDEF ?
        inline bool   isDefined(int i, int j) const
        { return hasValue(i, j) && getValue(i, j) != GRIB_NOTDEF; }

        // Reference date Date (file creation date)
        time_t getRecordRefDate () const         { return refDate; }
        const char* getStrRecordRefDate () const { return strRefDate; }

        // Date courante des prévisions
        time_t getRecordCurrentDate () const     { return curDate; }
        const char* getStrRecordCurDate () const { return strCurDate; }
        void  setRecordCurrentDate (time_t t);
        void   print();
        bool isFilled(){ return m_bfilled; }
        void setFilled(bool val=true){ m_bfilled = val;}

    protected:
    //private:
        double  *data;
        double *lons;
        double *lats;
        char   strRefDate [32];
        char   strCurDate [32];
        int    source;
        bool   m_bfilled;

        int Ni;
        int Nj;

        zuchar levelType;
        zuint  levelValue;

        char variableName[64];

        zuint  refyear, refmonth, refday, refhour, refminute;
        time_t refDate;      // C reference date
        time_t curDate;      // C current date

        time_t makeDate(zuint year,zuint month,zuint day,zuint hour,zuint min,zuint sec);
};
