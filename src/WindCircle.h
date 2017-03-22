#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include <cmath>
#include "wrdc.h"
#include "ocpn_plugin.h"
#include "GriddedData.h"
#include "VectorUtilities.h"
#include "Slocum.h"

class PlugIn_ViewPort;
class EnsembleForecast;


class WindCircleFactory {
public:

    WindCircleFactory();

    bool RenderGriddedForecast(wrDC &wrdc, PlugIn_ViewPort &vp,
                               EnsembleForecast *fcst, int time_idx,
                               int lon_idx, int lat_idx);

    void RenderWindTriangle(wxPoint center,
                            double speed,
                            double direction,
                            double radius,
                            wrDC &wrdc, PlugIn_ViewPort &vp);

    void RenderBackgroundCircle(wxPoint center,
                                double radius,
                                wrDC &wrdc, PlugIn_ViewPort &vp);

    void Reset() {};

    void ClearCached() {};

private:

    wxColour m_circle_fill;
};
