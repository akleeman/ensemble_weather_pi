#include <iostream>
#include "WindCircle.h"


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


void WindCircleFactory::RenderBackgroundCircle(wxPoint center,
                                               double radius,
                                               wrDC &wrdc, PlugIn_ViewPort &vp) {

    // Draw the circle background.
    wxBrush brush(m_circle_fill, wxALPHA_OPAQUE);
    wrdc.SetBrush(brush);
    wrdc.DrawCircle(center.x, center.y, radius);
}


void WindCircleFactory::RenderWindTriangle(wxPoint center,
                                           double speed,
                                           double direction,
                                           double radius,
                                           wrDC &wrdc, PlugIn_ViewPort &vp){

    double dir_ccw_from_east = 90. - 180 * direction / M_PI;

    int bin = bisect(wind_bins, speed);
    // Set the brush color for the given wind speed.
    wxBrush arc_brush(wind_colors[bin], wxALPHA_OPAQUE);
    wrdc.SetBrush(arc_brush);

    wrdc.DrawEllipticArc(center.x, center.y,
                         2 * radius, 2 * radius,
                         dir_ccw_from_east - 15.,
                         dir_ccw_from_east + 15.);
}


bool WindCircleFactory::RenderGriddedForecast(wrDC &wrdc, PlugIn_ViewPort &vp,
                                              EnsembleForecast *fcst,
                                              int time_idx, int lon_idx, int lat_idx){

    auto wind_speed = fcst->get_variable(WIND_SPEED_ID);
    auto wind_direction = fcst->get_variable(WIND_DIRECTION_ID);

    Tensor<double> dir = wind_direction.get_data();
    Tensor<double> speed = wind_speed.get_data();

    auto lons = wind_speed.get_lons();
    auto lats = wind_speed.get_lats();

    // Determine which radius to use by looking at the bouding box
    // of the visible forecast area.
    wxPoint ll, ur;
    get_bounding_box(vp, lons, lats, &ll, &ur);
    int visible_width = std::min(ur.x, vp.pix_width) - std::max(ll.x, 0);
    int visible_height = std::min(ur.y, vp.pix_height) - std::max(ll.y, 0);

    // TODO: perhaps here we should only be using the number of visible
    // longitudes and latitudes
    double x_radius = 0.3846 * visible_width / (float) lons->shape()[0];
    double y_radius = 0.3846 * visible_height / (float) lats->shape()[1];
    double radius = fmin(x_radius, y_radius);

    wxPoint pl;
    // Draw a red circle around the longitude / latitude that is currently
    // selected for spot forecasts
    wxBrush brush(wxColor("red"), wxALPHA_OPAQUE);
    wrdc.SetBrush(brush);
    double lon = lons->get(lon_idx, lat_idx);
    double lat = lats->get(lon_idx, lat_idx);
    GetCanvasPixLL(&vp, &pl, lat, lon);
    wrdc.DrawCircle(pl.x, pl.y, radius / 6.);

    int n_realizations = fcst->shape()[1];
    std::vector<double> one_point_speeds(n_realizations);
    std::vector<int> order;

    // Now loop through all the grid points and draw a wind circle on each one.
    for (int i=0; i <= lons->shape()[0]; i++){
        for (int j=0; j <= lons->shape()[1]; j++){
            float lat = lats->get(i, j);
            float lon = lons->get(i, j);

            GetCanvasPixLL(&vp, &pl, lat, lon);

            // First draw the background
            RenderBackgroundCircle(pl, radius, wrdc, vp);

            // Then determine the order of the ensembles, this allows
            // us to make sure the triangle showing the largest wind
            // speed is always shown.
            for (int k = 0; k < n_realizations; k++){
                one_point_speeds[k] = speed.get({time_idx, k, i, j});
            }
            order = argsort(one_point_speeds);

            // Draw each of the triangles
            for (int k = 0; k < speed.shape()[1]; k++){
                RenderWindTriangle(pl,
                                   speed.get({time_idx, order[k], i, j}),
                                   dir.get({time_idx, order[k], i, j}),
                                   radius, wrdc, vp);
            }
        }
    }
    return true;
}
