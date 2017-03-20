#include <iostream>
#include "WindCircle.h"


WindCircleFactory::WindCircleFactory()
      : m_circle_fill(255, 255, 255, 110)  // Each wind circle is a semi-transparent white fill.
{
    m_wind_colors[0].Set("#d7d7d7"); // light grey
    m_wind_colors[1].Set("#a1eeff"); // lightest blue
    m_wind_colors[2].Set("#42b1e5"); //light blue
    m_wind_colors[3].Set("#4277e5"); //pastel blue
    m_wind_colors[4].Set("#60fd4b"); //green
    m_wind_colors[5].Set("#1cea00"); //yellow-green
    m_wind_colors[6].Set("#fbef36"); //yellow
    m_wind_colors[7].Set("#fbc136"); //orange
    m_wind_colors[8].Set("#ff4f02"); //red
    m_wind_colors[9].Set("#d50c02"); //darker-red
    m_wind_colors[10].Set("#ff00c0"); //red-purple
    m_wind_colors[11].Set("#b30d8a"); //dark purple
    m_wind_colors[12].Set("#000000"); //black

    m_wind_bins = get_wind_bins();
}


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

    int bin = bisect(m_wind_bins, speed);
    wxColor color = m_wind_colors[bin];
    // Set the brush color for the given wind speed.
    wxBrush arc_brush(color, wxALPHA_OPAQUE);
    wrdc.SetBrush(arc_brush);

    wrdc.DrawEllipticArc(center.x, center.y,
                         2 * radius, 2 * radius,
                         dir_ccw_from_east - 15.,
                         dir_ccw_from_east + 15.);
}


bool WindCircleFactory::Render(wrDC &wrdc, PlugIn_ViewPort &vp,
                               EnsembleForecast *fcst, int time_idx){

    auto wind_speed = fcst->get_variable(WIND_SPEED_ID);
    auto wind_direction = fcst->get_variable(WIND_DIRECTION_ID);

    Tensor<double> dir = wind_direction.get_data();
    Tensor<double> speed = wind_speed.get_data();

    auto lons = wind_speed.get_lons();
    auto lats = wind_speed.get_lats();

    double radius = 20.;
    wxPoint pl;

    int n_realizations = fcst->shape()[1];
    std::vector<double> one_point_speeds(n_realizations);
    std::vector<int> order;

    for (int i=0; i <= lons->shape()[0]; i++){
        for (int j=0; j <= lons->shape()[1]; j++){
            float lat = lats->get(i, j);
            float lon = lons->get(i, j);

            GetCanvasPixLL(&vp, &pl, lat, lon);
            RenderBackgroundCircle(pl, radius, wrdc, vp);

            for (int k = 0; k < n_realizations; k++){
                one_point_speeds[k] = speed.get({time_idx, k, i, j});
            }
            order = argsort(one_point_speeds);

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
