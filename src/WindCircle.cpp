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


bool WindCircleFactory::Render(wrDC &wrdc, PlugIn_ViewPort &vp){

    wxPoint pl;
    for (int i=0; i <= 12; i++){
      GetCanvasPixLL(&vp, &pl, (float) -3 * i, 0.);

      wxBrush brush(m_circle_fill, wxALPHA_OPAQUE);
      wrdc.SetBrush(brush);

      wrdc.DrawCircle(pl.x, pl.y, 10.);

      wxBrush arc_brush(m_wind_colors[i], wxALPHA_OPAQUE);
      wrdc.SetBrush(arc_brush);

      wrdc.DrawEllipticArc(pl.x, pl.y, 2 * 10., 2 * 10., 30. * i, 30. * (i + 1));
    }
    return true;
}
