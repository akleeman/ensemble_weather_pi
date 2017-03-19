#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include "wrdc.h"
#include "ocpn_plugin.h"
#include "GriddedData.h"

class PlugIn_ViewPort;
class EnsembleForecast;

class WindCircleFactory {
public:

    WindCircleFactory();

    bool Render(wrDC &wrdc, PlugIn_ViewPort &vp, EnsembleForecast &fcst);

    void RenderOneCircle(wxPoint center, double angle_from, wxColor color,
                         double radius, wrDC &wrdc, PlugIn_ViewPort &vp);

    void Reset() {};

    void ClearCached() {};

private:

    wxColour m_circle_fill;
    wxColour m_wind_colors[13];
};
