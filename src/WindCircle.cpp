#include <iostream>
#include "WindCircle.h"


WindCircleFactory::WindCircleFactory()
      : m_circle_fill(255, 255, 255, 210)  // Each wind circle is a semi-transparent white fill.
{
}


bool WindCircleFactory::Render(wrDC &wrdc, PlugIn_ViewPort &vp){

    wxPoint pl;
    GetCanvasPixLL(&vp, &pl, 0., 0.);

    wxBrush brush(m_circle_fill, wxALPHA_OPAQUE);
    wrdc.SetBrush(brush);

    wrdc.DrawCircle(pl.x, pl.y, 30.);

    wxBrush arc_brush(wxColour(0, 0, 0), wxALPHA_OPAQUE);
    wrdc.SetBrush(arc_brush);

    wrdc.DrawEllipticArc(pl.x, pl.y, 2 * 30., 2 * 30., 0., 270.);

    return true;
}
