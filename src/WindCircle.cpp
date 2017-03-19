#include <iostream>
#include "WindCircle.h"


WindCircleFactory::WindCircleFactory()
      : m_circle_fill(255, 255, 255, 210)  // Each wind circle is a semi-transparent white fill.
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
}


void WindCircleFactory::RenderOneCircle(wxPoint center, double angle_from, wxColor color,
                                        double radius,
                                        wrDC &wrdc, PlugIn_ViewPort &vp) {

    wxBrush brush(m_circle_fill, wxALPHA_OPAQUE);
    wrdc.SetBrush(brush);

    wrdc.DrawCircle(center.x, center.y, 10.);

    wxBrush arc_brush(color, wxALPHA_OPAQUE);
    wrdc.SetBrush(arc_brush);

    wrdc.DrawEllipticArc(center.x, center.y,
                         2 * radius, 2 * radius,
                         angle_from - 11., angle_from + 11.);
}



bool WindCircleFactory::Render(wrDC &wrdc, PlugIn_ViewPort &vp, EnsembleForecast &fcst){

    auto wind_speed = fcst.get_variable(WIND_SPEED_ID);
    auto wind_direction = fcst.get_variable(WIND_DIRECTION_ID);
    Tensor<double> dir = wind_direction.get_data();
    Tensor<double> speed = wind_speed.get_data();

    auto lons = wind_speed.get_lons();
    auto lats = wind_speed.get_lats();

    wxPoint pl;
    std::cout << lons->shape()[0] << " : " << lons->shape()[1] << std::endl;
    for (int i=0; i <= lons->shape()[0]; i++){
        for (int j=0; j <= lons->shape()[1]; j++){

            float lat = lats->get(i, j);
            float lon = lons->get(i, j);

            GetCanvasPixLL(&vp, &pl, lat, lon);

            RenderOneCircle(pl, 45., m_wind_colors[0], 10., wrdc, vp);

        }
    }
    return true;
}
