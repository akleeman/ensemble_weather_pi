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

#include <iostream>
#include "WindCircle.h"


/*
 * Takes a grid of longitudes and latitudes and determines their
 * upper right and lower left most points in the current viewport.
 * Those values are stored in place in the ll and ur arguments.
 */
void get_bounding_box(PlugIn_ViewPort &vp,
                      Matrix<double> *lons,
                      Matrix<double> *lats,
                      wxPoint *ll, wxPoint *ur){

    wxPoint p;

    assert(lons->shape() == lats->shape());

    ll->x = vp.pix_width;
    ur->x = 0;
    ll->y = vp.pix_height;
    ur->y = 0;

    for (int i=0; i<lons->shape()[0]; i++){
        for (int j=0; j<lons->shape()[1]; j++){
            GetCanvasPixLL(&vp, &p, lats->get(i, j), lons->get(i, j));
            if (p.x < ll->x) ll->x = p.x;
            if (p.y < ll->y) ll->y = p.y;
            if (p.x > ur->x) ur->x = p.x;
            if (p.y > ur->y) ur->y = p.y;
        }
    }
}


WindCircleFactory::WindCircleFactory()
      : m_circle_fill(255, 255, 255, 80)  // Each wind circle is a semi-transparent white fill.
{};


/*
 * Draws a reference circle which makes it easier to tell
 * both where a forecast is valid and which direction
 * the wind is coming from.
 */
void WindCircleFactory::RenderBackgroundCircle(wxPoint center,
                                               double radius,
                                               wrDC &wrdc) {

    // Draw the circle background.
    wxBrush brush(m_circle_fill, wxALPHA_OPAQUE);
    wrdc.SetBrush(brush);
    wrdc.SetPen(wxNullPen);
    wrdc.DrawCircle(center.x, center.y, radius);
}


/*
 * Creates a pie slice like triangle representing a single
 * wind speed/direction forecast where the color indicates
 * the wind speed and the direction is given by the
 * orientation of the pie slice.
 */
void WindCircleFactory::RenderWindTriangle(wxPoint center,
                                           double speed,
                                           double direction,
                                           double radius,
                                           wrDC &wrdc){

    double dir_ccw_from_east = 90. - 180 * direction / M_PI;

    int bin = bisect(wind_bins, speed);
    // Set the brush color for the given wind speed.
    wxBrush arc_brush(wind_colors[bin], wxALPHA_OPAQUE);
    wrdc.SetBrush(arc_brush);
    wrdc.SetPen(wxNullPen);
    wrdc.DrawEllipticArc(center.x, center.y,
                         2 * radius, 2 * radius,
                         dir_ccw_from_east - 15.,
                         dir_ccw_from_east + 15.);
}


/*
 * Renders a wind circle at a given point on the screen.
 */
void WindCircleFactory::RenderPoint(wrDC &wrdc,
                                   wxPoint center,
                                   std::vector<double> speeds,
                                   std::vector<double> directions,
                                   double radius) {
    // First draw the background
    RenderBackgroundCircle(center, radius, wrdc);

    // Then determine the order of the ensembles, this allows
    // us to make sure the triangle showing the largest wind
    // speed is always shown.
    std::vector<int> order = argsort(speeds);

    // Draw each of the triangles
    for (int k = 0; k < (int) speeds.size(); k++){
        RenderWindTriangle(center,
                           speeds[order[k]],
                           directions[order[k]],
                           radius, wrdc);
    }
}


/*
 * Takes the wind values from an EnsembleForecast and renders the gridded
 * forecast corresponding to time `time_idx` to the screen.  The longitude
 * and latitude are required in order to surround the active point with a
 * circle indicating it's active.
 */
bool WindCircleFactory::RenderGriddedForecast(wrDC &wrdc, PlugIn_ViewPort &vp,
                                              EnsembleForecast *fcst,
                                              int time_idx, int lon_idx, int lat_idx){

    auto wind_speed = fcst->get_variable(WIND_SPEED_ID);
    auto wind_direction = fcst->get_variable(WIND_DIRECTION_ID);

    Tensor<double> dir = wind_direction.get_data();
    Tensor<double> speed = wind_speed.get_data();

    auto lons = wind_speed.get_lons();
    auto lats = wind_speed.get_lats();

    // Determine which radius to use by looking at the bounding box
    // of the visible forecast area.
    wxPoint ll, ur;
    get_bounding_box(vp, lons, lats, &ll, &ur);

    // The radius is then the overall width / height divided by the
    // number of points that need to be shown.  Note that the overall
    // width may be larger than the view port width, but that's ok because
    // not all the longitudes / latitudes will be shown either so the
    // ratios (visible_width / visible_lons) and (overall_width / all_lons)
    // should be very similar.
    int overall_width = ur.x - ll.x;
    int overall_height = ur.y - ll.y;
    double x_radius = 0.3846 * overall_width / (float) lons->shape()[0];
    double y_radius = 0.3846 * overall_height / (float) lats->shape()[1];
    double radius = fmin(x_radius, y_radius);

    wxPoint pl;
    // Draw a red circle around the longitude / latitude that is currently
    // selected for spot forecasts.
    wxPen pen(wxColor("red"));
    wrdc.SetPen(pen);
    wrdc.SetBrush(wxNullBrush);
    double lon = lons->get(lon_idx, lat_idx);
    double lat = lats->get(lon_idx, lat_idx);
    GetCanvasPixLL(&vp, &pl, lat, lon);
    wrdc.DrawCircle(pl.x, pl.y, radius);

    int n_realizations = fcst->shape()[1];
    std::vector<double> one_point_speeds(n_realizations);
    std::vector<double> one_point_directions(n_realizations);

    // Now loop through all the grid points and draw a wind circle on each one.
    for (int i=0; i < lons->shape()[0]; i++){
        for (int j=0; j < lons->shape()[1]; j++){
            float lat = lats->get(i, j);
            float lon = lons->get(i, j);

            GetCanvasPixLL(&vp, &pl, lat, lon);
            // Here we create vectors of the speeds and directions to pass into
            // the plotting function.
            for (int k = 0; k < n_realizations; k++){
                one_point_speeds[k] = speed.get({time_idx, k, i, j});
                one_point_directions[k] = dir.get({time_idx, k, i, j});
            }

            RenderPoint(wrdc, pl, one_point_speeds, one_point_directions, radius);

        }
    }
    return true;
}
