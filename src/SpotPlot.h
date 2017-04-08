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

#include <wx/wx.h>
#include "wrdc.h"
#include "Slocum.h"
#include "WindCircle.h"
#include "VectorUtilities.h"


class SpotPlot
{
    public:

        void create_grid(wrDC &wrdc, int n_x, int n_y);

        void add_x_label(wrDC &wrdc, SpotForecast *fcst);

        void draw_grid(wrDC &wrdc, SpotForecast *fcst);

        void draw_color_bar(wrDC &wrdc);

        void draw_probs(wrDC &wrdc, Matrix<double> probs);

        void get_wind_probs(SpotForecast *fcst, Matrix<double> *probs);

        void draw_time_marker(wrDC &wrdc, int time_index);

        void draw_current_time(wrDC &wrdc, SpotForecast *fcst);

        void draw_wind_circles(wrDC &wrdc, SpotForecast *fcst);

        void plot(wxWindow *window, SpotForecast *fcst, int time_index);

        wxWindow *get_window() { return m_window; };

        int get_time_index(const wxPoint &point);

        bool is_initialized();

    private:

        wxWindow *m_window;
        wrDC m_wrdc;
        double m_grid_height;
        double m_grid_width;
        double m_color_bar_width = 30;
        double m_header_height = 30;
        double m_x_label_height = 40;
        double m_y_label_width = 50;
        wxColour m_grid_color = wxColor(200, 200, 200);
        wxColour m_bg_color = wxColor(230, 230, 230);
        std::vector<double> m_grid_x, m_grid_y;
};
