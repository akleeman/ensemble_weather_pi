#include <iostream>
#include <wx/wx.h>

#include "wrdc.h"
#include "ocpn_plugin.h"
#include "Slocum.h"
#include "WindCircle.h"
#include "EnsembleWeatherUI.h"


class EnsembleWeatherManager : public EnsembleWeatherUI
{
  public:

    EnsembleWeatherManager(wxWindow* parent,
                           wxWindowID id = wxID_ANY,
                           const wxString& title = wxT("Ensemble Weather"),
                           const wxPoint& pos = wxDefaultPosition,
                           const wxSize& size = wxSize( 389,62 ),
                           long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
    : EnsembleWeatherUI(parent, id, title, pos, size, style) {
        m_fcst = read_slocum_forecast("/home/kleeman/dev/slocum/queries/east_atlantic_query.fcst");
    };

    void OnOpenFile(wxCommandEvent& event);

    void OnPaintSpot(wxPaintEvent& event);

    void Reset();

    void Render(wrDC &wrdc, PlugIn_ViewPort &vp);

  private:

    EnsembleForecast m_fcst;
    WindCircleFactory m_wind_circle_factory;
};
